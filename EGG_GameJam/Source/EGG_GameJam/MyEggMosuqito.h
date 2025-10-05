// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEggMosuqito.generated.h"

UCLASS()
class EGG_GAMEJAM_API AMyEggMosuqito : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyEggMosuqito();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	AActor* TargetEgg;

	UPROPERTY()
	bool bHasEgg = false;

	UPROPERTY()
	FVector LerpAwayDirection;

	UPROPERTY()
	float LerpSpeed = 300.f; // units per sec

};
