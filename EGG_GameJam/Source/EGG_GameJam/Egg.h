// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IBoil.h"
#include "Interact.h"

#include "Egg.generated.h"

UCLASS()
class EGG_GAMEJAM_API AEgg : public AActor, public IIBoil, public IInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEgg();
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egg" )
	bool bDebug = false;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egg" )
	float EggCookingTemperature = 75;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egg" )
	float PerfectEatingTemperatureRange = 20;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egg" )
	float PerfectEatingTemperature = 60;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egg" )
	float PerfectCookTimeSecondsRange = 30.0f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egg" )
	float PerfectCookTimeSeconds = 60.0f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egg" )
	float MaxBonusPoints = 100.0f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egg" )
	float BasePoints = 10.0f;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egg | Base" )
	float TemperatureSinkSpeed = 1;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egg | Base" )
	float TemperatureRiseSpeed = 1;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egg | Base" )
	float MaxTemperature = 100;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egg | Base" )
	float MinTemperature = 30;
	
	float EatEgg();
	virtual void Boiling_Implementation( bool _enter ) override;
	virtual bool Interact_Implementation( FVector2D _input,  FVector _pos ) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float CurrentCookTime = 0;
	float CurrentTemperature = 20;
	bool IsBoiling = false;

	int SimulationCounter = -1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	void EggBoilCalculation( const float _deltaTime );

	void DrawDebug();
};
