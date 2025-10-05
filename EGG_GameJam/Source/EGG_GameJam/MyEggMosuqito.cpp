// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEggMosuqito.h"

#include "Egg.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AMyEggMosuqito::AMyEggMosuqito()
{
	PrimaryActorTick.bCanEverTick = true;

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	float Radius = 10.f;
	FVector RandomPoint = FMath::VRand() * Radius;
	RandomPoint.Y = abs(RandomPoint.Y);
	if (PlayerCharacter)
	{
		SetActorLocation( RandomPoint + PlayerCharacter->GetActorLocation());
	}
}

void AMyEggMosuqito::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Eggs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEgg::StaticClass(), Eggs);
	if (Eggs.Num() == 0)
	{
		Destroy();
		return;
	}
	int ranomEgg = rand() % Eggs.Num();

	TargetEgg = Eggs[ranomEgg];
}

void AMyEggMosuqito::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!TargetEgg)
	{	
		Destroy();
		return;
	}

	if (bKillTimerActive)
	{
		KillTimer -= DeltaTime;
		if (KillTimer <= 0)
		{
			Destroy();
			return;
		}
	}
	
	FVector CurrentLocation = GetActorLocation();
	if (!bHasEgg)
	{
		FVector TargetLocation = TargetEgg->GetActorLocation();
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, LerpSpeed);
		SetActorLocation(NewLocation);
		
		if (FVector::Dist(NewLocation, TargetLocation) < 10.f) 
		{
			TargetEgg->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			TargetEgg->DisableComponentsSimulatePhysics();
			bHasEgg = true;
			
			LerpAwayDirection = FVector(FMath::FRandRange(-1.f, 1.f),
				FMath::FRandRange(-1.f, 1.f), 1.f).GetSafeNormal();
		}
	}
	else
	{
		FVector NewLocation = GetActorLocation() + LerpAwayDirection * LerpSpeed * DeltaTime;
		SetActorLocation(NewLocation);
		bKillTimerActive = true;
	}
}

