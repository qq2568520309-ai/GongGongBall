// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/BallBase.h"

#include "Ball/HandBall.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ABallBase::ABallBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
}

void ABallBase::RespondToOverlap(AActor* Actor)
{
	AHandBall* Ball = Cast<AHandBall>(Actor);
	if (Ball == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp,Warning,TEXT("Velocity = %s"),*Ball->Velocity.ToString());
	FVector ToBall =
		GetActorLocation() - Ball->GetActorLocation();
	FVector N = ToBall.GetSafeNormal();
	FVector HandVel = Ball->Velocity;
	float Speed = HandVel.Size();
	if (Speed < 5.f) // 静止手壶 = 当墙
	{
		//完全弹性反射
		Velocity =
			Velocity
			- 2.f * FVector::DotProduct(Velocity, N) * N;

		Velocity *= 0.95f;
		// 防止卡住
		AddActorWorldOffset(N * 2.f, false);
	}
	
	else
	{
		FVector RelVel = HandVel - Velocity;
		float ClosingSpeed = FVector::DotProduct(RelVel, N);
		if (ClosingSpeed > 0.f)
		{
			Velocity += N * ClosingSpeed * 1.5f;
		}
	}
	
	if (Velocity.Length() >= MaxVelocity)
	{
		Velocity = Velocity.GetClampedToMaxSize(MaxVelocity);
	}
	//Z轴方向不能有速度
	Velocity.Z = 0;
	
	
}

// Called when the game starts or when spawned
void ABallBase::BeginPlay()
{
	Super::BeginPlay();
	Velocity = InitialVelocity;
	
}


void ABallBase::Tick(float DeltaTime)
{
	if (!HasAuthority())
	{
		return;
	}

	FVector DeltaMove = Velocity * DeltaTime;
	
	ServerUpdateLocation(DeltaMove);
	
	TArray<AActor*> Overlaps;
	GetOverlappingActors(Overlaps);
	
	//限速
	if (Velocity.Length() >= MaxVelocity)
	{
		Velocity = Velocity.GetClampedToMaxSize(MaxVelocity);
	}
	//Z轴方向不能有速度
	Velocity.Z = 0;
	
	
}

void ABallBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABallBase, Velocity);
}

void ABallBase::ServerUpdateLocation_Implementation(FVector DeltaMove)
{
	FHitResult Hit;
	AddActorWorldOffset(DeltaMove, true, &Hit);
	if (Hit.bBlockingHit)
	{
		FVector N = Hit.ImpactNormal;

		// 反弹（墙）
		Velocity = Velocity
			- 2.f * FVector::DotProduct(Velocity, N) * N;

		Velocity *= 0.95f;

		// 防止贴墙抖动
		AddActorWorldOffset(N * 1.0f, false);
	}
}

