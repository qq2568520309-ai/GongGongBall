// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainBallGamemode.generated.h"

class AHandBall;
class ASpawnPlaceBase;
class ABallBase;
class ABallPlayer;
UCLASS()
class GONGGONGBALL_API AMainBallGamemode : public AGameMode
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<AActor*> AvailableSpawns;
	UPROPERTY()
	TArray<AActor*> UsedSpawns;
	//玩家
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<ABallPlayer> BallPlayerClass;
	//冰球
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABallBase> BallBaseClass;
	//手壶
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AHandBall> HandBallClass;
	
	int CurrentPlayerIndex = 0;
	
	//组播生成角色
	void SpawnBallPlayer(APlayerController* PlayerController);
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
protected:
	virtual void BeginPlay()override;
	
	
	
	
	
	
};
