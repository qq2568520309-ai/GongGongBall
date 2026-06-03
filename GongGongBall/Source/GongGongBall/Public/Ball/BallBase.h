// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallBase.generated.h"

UCLASS()
class GONGGONGBALL_API ABallBase : public AActor
{
	GENERATED_BODY()

public:
	ABallBase();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector InitialVelocity;
	UPROPERTY(BlueprintReadOnly)
	FVector Velocity;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
