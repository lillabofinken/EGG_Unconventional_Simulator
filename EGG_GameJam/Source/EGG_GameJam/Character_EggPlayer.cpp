// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_EggPlayer.h"

class UEnhancedInputLocalPlayerSubsystem;

ACharacter_EggPlayer::ACharacter_EggPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacter_EggPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacter_EggPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacter_EggPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void Move(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	if (Controller)
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, Input.Y);
		AddMovementInput(Right, Input.X);
	}
}

void Look(const FInputActionValue& Value)
{
	
}