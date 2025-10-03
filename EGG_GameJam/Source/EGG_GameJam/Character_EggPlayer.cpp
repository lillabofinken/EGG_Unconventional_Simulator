// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_EggPlayer.h"

class UEnhancedInputLocalPlayerSubsystem;

ACharacter_EggPlayer::ACharacter_EggPlayer():
PlayerMappingContext(nullptr),
MoveAction(nullptr),
InteractAction(nullptr),
ChangeMoveAxisAction(nullptr),
HandActor(nullptr),
MoveSpeed(1.0f),
bInteract(false),
bDepthMode(false)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacter_EggPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(PlayerMappingContext, 0);
			}
		}
	}

	if (HandBP)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		HandActor = GetWorld()->SpawnActor<AActor>(
			HandBP,
			FVector(0.f, 0.f, 0.f),
			FRotator::ZeroRotator,
			SpawnParams
		);
		HandActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HandSocket"));
	}
}

void ACharacter_EggPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacter_EggPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacter_EggPlayer::Move);
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &ACharacter_EggPlayer::InteractStarted);
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Completed, this, &ACharacter_EggPlayer::InteractCompleted);
		EnhancedInput->BindAction(ChangeMoveAxisAction, ETriggerEvent::Started, this, &ACharacter_EggPlayer::ChangeMoveAxisStarted);
		EnhancedInput->BindAction(ChangeMoveAxisAction, ETriggerEvent::Completed, this, &ACharacter_EggPlayer::ChangeMoveAxisCompleted);
	}
}

void ACharacter_EggPlayer::Move(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	if (HandActor)
	{
	// GEngine->AddOnScreenDebugMessage(10, 0.0f, FColor::Green, FString::Printf(
	// 	TEXT("Look Input: X=%f, Y=%f"), Input.X, Input.Y));
		FVector CurrentLocation = HandActor->GetActorLocation();
		FVector NewLocation = CurrentLocation;

		if (!bDepthMode)
		{
			NewLocation.X += Input.Y * MoveSpeed;
			NewLocation.Y += Input.X * MoveSpeed;
		}
		else
		{
			NewLocation.Z -= Input.Y * MoveSpeed;
			NewLocation.Y += Input.X * MoveSpeed;
		}

		HandActor->SetActorLocation(NewLocation);

		GEngine->AddOnScreenDebugMessage(10, 0.0f, FColor::Green, FString::Printf(
	TEXT("New Location: X=%f, Y=%f"), NewLocation.X, NewLocation.Y));
	}
}

void ACharacter_EggPlayer::InteractStarted(const FInputActionValue& Value)
{
	bInteract = true;
}

void ACharacter_EggPlayer::InteractCompleted(const FInputActionValue& Value)
{
	bInteract = false;
}

void ACharacter_EggPlayer::ChangeMoveAxisStarted(const FInputActionValue& Value)
{
	bDepthMode = true;
}

void ACharacter_EggPlayer::ChangeMoveAxisCompleted(const FInputActionValue& Value)
{
	bDepthMode = false;
}