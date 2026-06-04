// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "BallBase.generated.h"

UCLASS()
class GONGGONGBALL_API ABallBase : public AActor
{
	GENERATED_BODY()

public:
	ABallBase();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector InitialVelocity = FVector(100.0f, 0.0f, 0.0f);
	UPROPERTY(Replicated, BlueprintReadOnly)
	FVector Velocity;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxVelocity = 1000;
	
	void RespondToOverlap(AActor* Actor);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(Server, Reliable)
	void ServerUpdateLocation(FVector DeltaMove);
};
