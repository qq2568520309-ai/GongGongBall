// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/HandBall.h"
#include "Ball/BallBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AHandBall::AHandBall()
{
	PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    AActor::SetReplicateMovement(true);
}



void AHandBall::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHandBall::Tick(float DeltaTime)
{
    if (!IsLocallyControlled())return;
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
    
    // 由服务端权威移动，并使用客户端提交的瞬时速度驱动碰撞响应。
    ServerUpdateLocation(DesiredMove, Velocity);
    
    
    
}

void AHandBall::ServerUpdateLocation_Implementation(FVector DesiredMove, FVector InVelocity)
{
    UE_LOG(LogTemp, Warning, TEXT("RPC_UpdateLocationCalled!"));

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
    
}

void AHandBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHandBall, Velocity);
}

bool AHandBall::IsLocallyControlled()
{
    if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
    {
        return PC->IsLocalController();
    }
    return false;
}

