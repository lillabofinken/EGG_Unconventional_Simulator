// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyEggMosuqito.h"
#include "GameFramework/Actor.h"
#include "MymoswquitoMAnager.generated.h"

UCLASS()
class EGG_GAMEJAM_API AMymoswquitoMAnager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMymoswquitoMAnager();

	virtual void Tick(float DeltaSeconds) override;
	
	void AddEggMosuqito();

	UPROPERTY(EditAnywhere, Category="Mosquito")
	TSubclassOf<AMyEggMosuqito> MosquitoBP;

	UPROPERTY()
	TArray<AMyEggMosuqito*> Mosquitos;

	UPROPERTY(EditAnywhere, Category="Mosquito")
	float SpawnTimeMin;
	UPROPERTY(EditAnywhere, Category="Mosquito")
	float SpawnTimeMax;
	
private:
	float SpawnTimer;
};
