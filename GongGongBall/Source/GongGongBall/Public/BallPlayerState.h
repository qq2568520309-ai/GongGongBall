// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BallPlayerState.generated.h"


UCLASS()
class GONGGONGBALL_API ABallPlayerState : public APlayerState
{
	GENERATED_BODY()
	public:
	UPROPERTY(BlueprintReadWrite)
	int Health;
};
