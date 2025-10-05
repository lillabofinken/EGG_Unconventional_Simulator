// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact.h"

#include "FishingRod.generated.h"

UCLASS()
class EGG_GAMEJAM_API AFishingRod : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFishingRod();

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egging | Reel" )
	bool bDebugPrint = false;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egging | Reel" )
	float ReelingFailPenalty = 2.0f;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egging | Reel" )
	float ReelSpeedDecrease = 1.0f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egging | Reel" )
	float ReelingSpeedIncrease = 0.1f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egging | Reel" )
	float ReelIncreaseAmount = 0.1f;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egging | Reel" )
	float ReelingSpeedCurve = 2.0f;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Egging | Rod" )
	float RodMoveSpeed = 1.0f;

		UFUNCTION( BlueprintCallable )
		void ReelIn( FVector2D _input );
	
		UFUNCTION( BlueprintCallable )
		float getLineLenght(){ return LineLenght; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float PenaltyTimer = 0.0f;

	UPROPERTY( BlueprintReadWrite )
	float LineLenght = 0;

	UPROPERTY( BlueprintReadOnly )
	float ReelingAmount = 0.0;
	
	UPROPERTY( BlueprintReadOnly )
	FVector2D RodDirection = FVector2D( 0, 0 ); 




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool Interact_Implementation( FVector2D _input, FVector _pos ) override;

private:
	int inputIndex = -1;



public:
	UFUNCTION( BlueprintCallable )
	static AFishingRod* GetFishingRod();
private:
	inline static AFishingRod* FishingRod = nullptr;
};
