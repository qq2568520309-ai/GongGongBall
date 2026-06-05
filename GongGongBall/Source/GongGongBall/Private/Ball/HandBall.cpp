// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/HandBall.h"
#include "Ball/BallBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AHandBall::AHandBall()
{
	PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    AActor::SetReplicateMovement(false);
}



void AHandBall::BeginPlay()
{
	Super::BeginPlay();

    ServerLocation = GetActorLocation();
}

void AHandBall::Tick(float DeltaTime)
{
    const bool bLocalControl = IsLocallyControlled();

    if (!bLocalControl)
    {
        if (HasAuthority())
        {
            ServerLocation = GetActorLocation();
        }

        SmoothToServerState(DeltaTime);
        return;
    }

    if (DeltaTime <= KINDA_SMALL_NUMBER)return;

    FVector OldLocation = GetActorLocation();
    FVector WorldLocation;
    FVector WorldDirection;
    
    if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
    {
        if (!PC->DeprojectMousePositionToWorld(
            WorldLocation, 
            WorldDirection))return;
        
    }
    
    FPlane TablePlane(
        FVector(0,0,OldLocation.Z),
        FVector::UpVector
    );
    
    FVector MousePos =
        FMath::LinePlaneIntersection(
            WorldLocation,
            WorldLocation + WorldDirection * 100000.f,
            TablePlane
        );
    
    FVector DesiredMove =
        FMath::VInterpTo(
            OldLocation,
            MousePos,
            DeltaTime,
            InterpSpeed
        ) - OldLocation;
    
    Velocity = DesiredMove / DeltaTime;
    Velocity.Z = 0.f;

    // 客户端先本地预测移动，避免只能等待服务端复制导致的抖动。
    FHitResult LocalHit;
    AddActorWorldOffset(DesiredMove, true, &LocalHit);
    if (LocalHit.bBlockingHit)
    {
        FVector LocalSlide = FVector::VectorPlaneProject(
            DesiredMove,
            LocalHit.ImpactNormal
        );
        AddActorWorldOffset(LocalSlide, true);
    }
    
    // 由服务端权威移动，并使用客户端提交的瞬时速度驱动碰撞响应。
    if (!HasAuthority())
    {
        //发送RPC
        ServerUpdateLocation(DesiredMove, Velocity);
    }
    else
    {
        //服务器直接执行具体内容
        ServerUpdateLocation_Implementation(DesiredMove, Velocity);
    }

    SmoothToServerState(DeltaTime);
}

void AHandBall::ServerUpdateLocation_Implementation(FVector DesiredMove, FVector InVelocity)
{
    //服务器更新该手壶的速度
    Velocity = InVelocity;
    Velocity.Z = 0.f;
    
    FHitResult Hit;
    AddActorWorldOffset(DesiredMove,true,&Hit);
    //直接撞上了冰球
    if (Cast<ABallBase>(Hit.GetActor())!=nullptr)
    {
        Cast<ABallBase>(Hit.GetActor())->RespondToOverlap(this);
    }
    //滑动，不要撞墙就停住了
    if (Hit.bBlockingHit)
    {
        FVector Slide = FVector::VectorPlaneProject(
            DesiredMove,
            Hit.ImpactNormal
        );
        AddActorWorldOffset(Slide, true);
    }

    ServerLocation = GetActorLocation();
}

void AHandBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHandBall, Velocity);
    DOREPLIFETIME(AHandBall, ServerLocation);
}

bool AHandBall::IsLocallyControlled()
{
    if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
    {
        return PC->IsLocalController();
    }
    return false;
}

void AHandBall::SmoothToServerState(float DeltaTime)
{
    if (DeltaTime <= KINDA_SMALL_NUMBER)
    {
        return;
    }

    const FVector CurrentLocation = GetActorLocation();
    const FVector DeltaToServer = ServerLocation - CurrentLocation;
    const float ErrorDistance = DeltaToServer.Size();
    if (ErrorDistance <= KINDA_SMALL_NUMBER)
    {
        return;
    }

    if (ErrorDistance >= TeleportCorrectionDistance)
    {
        SetActorLocation(ServerLocation);
        return;
    }

    FVector TargetLocation = ServerLocation;
    if (IsLocallyControlled() && ErrorDistance > MaxSmoothCorrectionDistance)
    {
        TargetLocation = CurrentLocation + DeltaToServer.GetClampedToMaxSize(MaxSmoothCorrectionDistance);
    }

    const FVector SmoothedLocation = FMath::VInterpTo(
        CurrentLocation,
        TargetLocation,
        DeltaTime,
        CorrectionInterpSpeed
    );
    SetActorLocation(SmoothedLocation);
}
