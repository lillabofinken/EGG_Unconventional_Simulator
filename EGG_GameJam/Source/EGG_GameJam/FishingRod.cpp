// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingRod.h"


// Sets default values
AFishingRod::AFishingRod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FishingRod = this;
}

// Called when the game starts or when spawned
void AFishingRod::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFishingRod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if( bDebugPrint )
	{
		GEngine->AddOnScreenDebugMessage( 1343001, 5, FColor::Magenta, "ReelingAmount:" + FString::SanitizeFloat( ReelingAmount )  );
		GEngine->AddOnScreenDebugMessage( 1343002, 5, FColor::Magenta, "PenaltyTimer :" + FString::SanitizeFloat( PenaltyTimer )  );
		GEngine->AddOnScreenDebugMessage( 1343003, 5, FColor::Magenta, "inputIndex   :" + FString::SanitizeFloat( inputIndex )  );
	}

	ReelingAmount -= DeltaTime * ReelSpeedDecrease;
	if( ReelingAmount < 0 )
		ReelingAmount = 0;
	
	if( PenaltyTimer > 0 )
		PenaltyTimer -= DeltaTime;
		return;
}

void AFishingRod::Interact_Implementation( FVector2D _input )
{
	if( PenaltyTimer > 0 )
		return;
	
	  // 0 = FAIL
	 //  1 = NO_CHANGE
	//   2 = SUCCESS
	int ReelingState = 1;
	
	const bool W = _input.Y >  0.5;
	const bool S = _input.Y < -0.5;
	 //  W
	// A S D
	const bool A = _input.X < -0.5;
	const bool D = _input.X >  0.5;

	int currentInputIndex = -1;


	
	if     ( W && !D && !S )
		currentInputIndex = 0;
	else if( D && !S && !A )
		currentInputIndex = 1;
	else if( S && !A && !W )
		currentInputIndex = 2;
	else if( A && !W && !D )
		currentInputIndex = 3;

	
	GEngine->AddOnScreenDebugMessage( 13444003, 5, FColor::Magenta, "currentInputIndex   :" + FString::SanitizeFloat( currentInputIndex )  );

	if( inputIndex == -1 && currentInputIndex != -1 )
		ReelingState = 2;
	else if( currentInputIndex == inputIndex )
		ReelingState = 1;
	else if( currentInputIndex == inputIndex + 1 )
		ReelingState = 2;
	else if( inputIndex == 3 && currentInputIndex == 0 )
		ReelingState = 2;
	else
		ReelingState = 0;

	if( ReelingState == 0 )
	{
		PenaltyTimer = ReelingFailPenalty;
		ReelingAmount -= ( ReelingAmount / 4 ) * 3;
		inputIndex = -1;
	}
	else if( ReelingState == 2 )
	{
		ReelingAmount += ReelingSpeedIncrease;
		if( ReelingAmount > 1 )
			ReelingAmount = 1;

		inputIndex = currentInputIndex;
	}
		
	
	IInteract::Interact_Implementation( _input );
}

AFishingRod* AFishingRod::GetFishingRod()
{
	return FishingRod;
}

