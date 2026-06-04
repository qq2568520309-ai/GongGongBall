#include "MainBallGamemode.h"

#include "BallPlayer.h"
#include "GameFramework/PlayerStart.h"
#include "SpawnPlaceBase.h"
#include "MainBallController.h"
#include "Ball/BallBase.h"
#include "Ball/HandBall.h"
#include "Kismet/GameplayStatics.h"



void AMainBallGamemode::BeginPlay()
{
	Super::BeginPlay();
	
	if (AvailableSpawns.IsEmpty() && UsedSpawns.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(),AvailableSpawns );
	}
	//生成冰球
	for (AActor* Actor : AvailableSpawns)
	{
		ASpawnPlaceBase* SpawnPlace = Cast<ASpawnPlaceBase>(Actor);
		if (SpawnPlace != nullptr)
		{
			if (SpawnPlace->SpawnType== ESpawnType::IceBall)
			{
				FActorSpawnParameters SpawnParams;
				GetWorld()->SpawnActor<ABallBase>(
				BallBaseClass,
				Actor->GetActorLocation(),
				Actor->GetActorRotation(),
				SpawnParams
				);
				
			}
		}
		
	}
	
}

void AMainBallGamemode::SpawnBallPlayer(APlayerController* PlayerController)
{
	AMainBallController* PC  = Cast<AMainBallController>(PlayerController);
	
	if (PC != nullptr)
	{
		for (AActor* Actor : AvailableSpawns)
		{
			ASpawnPlaceBase* SpawnPlace = Cast<ASpawnPlaceBase>(Actor);
			if (SpawnPlace != nullptr)
			{
				//生成玩家
				if (SpawnPlace->SpawnType == ESpawnType::Player && CurrentPlayerIndex == SpawnPlace->PlayerIndex)
				{
					//生成玩家
					UsedSpawns.Add(Actor);
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = PC;
					ABallPlayer* Player = GetWorld()->SpawnActor<ABallPlayer>(
					BallPlayerClass,
					Actor->GetActorLocation(),
					Actor->GetActorRotation(),
					SpawnParams
					);
					PC->Possess(Player);
					PC->BallPlayer = Player;
				}
				else if (SpawnPlace->SpawnType == ESpawnType::HandBall && CurrentPlayerIndex == SpawnPlace->PlayerIndex)
				{
					//生成手壶
					UsedSpawns.Add(Actor);
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = PC;
					AHandBall* HandBall = GetWorld()->SpawnActor<AHandBall>(
					HandBallClass,
					Actor->GetActorLocation(),
					Actor->GetActorRotation(),
					SpawnParams
					);
					PC->HandBall = HandBall;
					
				}
			}
		}
		CurrentPlayerIndex++;
		
	}
	
	
}

void AMainBallGamemode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (AvailableSpawns.IsEmpty() && UsedSpawns.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),ASpawnPlaceBase::StaticClass(),AvailableSpawns );
	}
	SpawnBallPlayer(NewPlayer);
	
}


