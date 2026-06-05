// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "HandBall.generated.h"

UCLASS()
class GONGGONGBALL_API AHandBall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHandBall();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LineLength = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InterpSpeed = 30;
	UPROPERTY(Replicated, BlueprintReadOnly)
	FVector Velocity;
	UPROPERTY(Replicated)
	FVector ServerLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CorrectionInterpSpeed = 12.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSmoothCorrectionDistance = 120.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TeleportCorrectionDistance = 250.f;
	
	
	
	
protected:
	virtual void BeginPlay() override;
	

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(Server,Reliable)
	void ServerUpdateLocation(FVector DesiredMove, FVector InVelocity);
	
	bool IsLocallyControlled();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SmoothToServerState(float DeltaTime);
	
};
