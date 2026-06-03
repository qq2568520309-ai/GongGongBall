// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/BallBase.h"
#include "Ball/HandBall.h"

// Sets default values
ABallBase::ABallBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Velocity = InitialVelocity;
	
}

// Called when the game starts or when spawned
void ABallBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABallBase::Tick(float DeltaTime)
{
	FVector DeltaMove = Velocity * DeltaTime;

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
	
	TArray<AActor*> Overlaps;
	GetOverlappingActors(Overlaps);

	for (AActor* Actor : Overlaps)
	{
		AHandBall* Hand = Cast<AHandBall>(Actor);
		if (!Hand) continue;
		FVector ToBall =
			GetActorLocation() - Hand->GetActorLocation();
		FVector N = ToBall.GetSafeNormal();
		FVector HandVel = Hand->Velocity;
		float Speed = HandVel.Size();
		if (Speed < 5.f) // 静止手壶 = 当墙
		{
			// ✔ 完全弹性反射
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
	}
	
}

