// Fill out your copyright notice in the Description page of Project Settings.


#include "Egg.h"

// Sets default values
AEgg::AEgg()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

float AEgg::EatEgg()
{
	float eggScore = 0.1f;

	float TemperatureDistance =  abs( PerfectEatingTemperature - CurrentTemperature );
	TemperatureDistance = FMath::Clamp( TemperatureDistance ,0, PerfectEatingTemperatureRange );
	TemperatureDistance /= PerfectEatingTemperatureRange;
	TemperatureDistance = 1 - TemperatureDistance;
	TemperatureDistance = TemperatureDistance * 0.6f + 0.4f;

	float CookDistance = abs( PerfectCookTimeSeconds - CurrentCookTime );
	CookDistance = FMath::Clamp( CookDistance, 0, PerfectCookTimeSecondsRange );
	CookDistance /= PerfectCookTimeSecondsRange;
	CookDistance = 1 - CookDistance;

	const float Points = MaxBonusPoints * TemperatureDistance * CookDistance + BasePoints;
	return Points;
}

void AEgg::Boiling_Implementation( bool _enter )
{
	IsBoiling = _enter;
}

// Called when the game starts or when spawned
void AEgg::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEgg::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EggBoilCalculation( DeltaTime );
	DrawDebug();
}

void AEgg::EggBoilCalculation( const float _deltaTime )
{
	if( CurrentTemperature > MinTemperature || IsBoiling )
		CurrentTemperature += TemperatureRiseSpeed * _deltaTime * ( IsBoiling ? TemperatureRiseSpeed : -TemperatureSinkSpeed );
	CurrentTemperature = FMath::Clamp( CurrentTemperature, 0, MaxTemperature );
		
	if( CurrentTemperature > EggCookingTemperature )
	{
		CurrentCookTime += _deltaTime;
		if( CurrentCookTime > MaxTemperature )
			CurrentCookTime = MaxTemperature;
	}
}

void AEgg::DrawDebug()
{
	if( !bDebug )
		return;
	
	GEngine->AddOnScreenDebugMessage( 10000001, 5, FColor::Green, "CurrentCookTime: " + FString::SanitizeFloat( CurrentCookTime ) );
	GEngine->AddOnScreenDebugMessage( 10000002, 5, FColor::Green, "CurrentTemperature: " + FString::SanitizeFloat( CurrentTemperature ) );
	GEngine->AddOnScreenDebugMessage( 10000003, 5, FColor::Green, "EatEgg: " + FString::SanitizeFloat( EatEgg() ) );
	GEngine->AddOnScreenDebugMessage( 10000004, 5, IsBoiling ? FColor::Green : FColor::Red, "IsBoiling: " + FString( IsBoiling ? "TRUE" : "FALSE" ) );
}