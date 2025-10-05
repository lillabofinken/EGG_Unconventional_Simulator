// Fill out your copyright notice in the Description page of Project Settings.


#include "MymoswquitoMAnager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

AMymoswquitoMAnager::AMymoswquitoMAnager(): 
SpawnTimeMin(4),
SpawnTimeMax(10),
SpawnTimer(2)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMymoswquitoMAnager::Tick(float DeltaSeconds)
{
	AActor::Tick(DeltaSeconds);

	SpawnTimer -= DeltaSeconds;
	if (SpawnTimer <= 0)
	{
		AddEggMosuqito();
		float SpawnTime = FMath::RandRange(SpawnTimeMin, SpawnTimeMax);
		SpawnTimer = SpawnTime;
	}
	GEngine->AddOnScreenDebugMessage(135, 0.0f, FColor::Green,
FString::Printf(TEXT("Spawn timer: %f"), SpawnTimer));
}

void AMymoswquitoMAnager::AddEggMosuqito()
{
	if (!MosquitoBP) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AMyEggMosuqito* SpawnedMosquito = GetWorld()->SpawnActor<AMyEggMosuqito>(MosquitoBP,
		FVector(0.0f), FRotator(0.0f), SpawnParams);
	GEngine->AddOnScreenDebugMessage(1335, 2.0f, FColor::Green,
	FString::Printf(TEXT("Spawned moosqitooo")));
	if (SpawnedMosquito)
	{
		Mosquitos.Add(SpawnedMosquito);
	}
}
