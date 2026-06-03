// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	UPROPERTY(BlueprintReadOnly)
	FVector Velocity;
	
	
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
};
