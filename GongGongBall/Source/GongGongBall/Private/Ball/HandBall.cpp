// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/HandBall.h"


// Sets default values
AHandBall::AHandBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AHandBall::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHandBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller != nullptr)
	{
		FHitResult Hit;
		FVector WorldLocation;
		FVector WorldDirection;
		Controller->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
		FCollisionObjectQueryParams ObjParams;
		//追踪静态和动态物体
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		GetWorld()->LineTraceSingleByObjectType(Hit, WorldLocation, WorldLocation+WorldDirection*LineLength,ObjParams);
		FVector MousePos = Hit.Location;
		MousePos.Z = GetActorLocation().Z;
		
		
		if ((MousePos-GetActorLocation()).Length() > 1.0f)
		{
			Velocity = (MousePos-GetActorLocation())/DeltaTime;
			SetActorLocation(
			FMath::VInterpTo(
				GetActorLocation(),
				MousePos,
				DeltaTime,
				InterpSpeed
				),
				true
				);
		}
		else
		{
			Velocity = FVector::ZeroVector;
		}
		
		
	}
}

