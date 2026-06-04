// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPlaceBase.generated.h"

UENUM(BlueprintType)
enum class ESpawnType:uint8
{
	Player,
	HandBall,
	IceBall,
};

UCLASS()
class GONGGONGBALL_API ASpawnPlaceBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	ESpawnType SpawnType;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int PlayerIndex = 0;
};
