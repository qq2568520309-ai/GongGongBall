// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainBallController.generated.h"


class AHandBall;
class ABallPlayer;
UCLASS()
class GONGGONGBALL_API AMainBallController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ABallPlayer> BallPlayer;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AHandBall> HandBall;
	UPROPERTY(BlueprintReadWrite)
	int PlayerIndex;
	
protected:
	
	virtual void BeginPlay() override;
	

};
