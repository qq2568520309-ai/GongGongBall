// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bound.generated.h"

UENUM()
enum class EBoundType: uint8
{
	MoveBound,
	WinBound
};

UCLASS()
class GONGGONGBALL_API ABound : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABound();
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EBoundType BoundType;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int PlayerIndex;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
