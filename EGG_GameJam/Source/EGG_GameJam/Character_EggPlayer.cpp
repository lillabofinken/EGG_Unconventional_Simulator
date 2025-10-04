// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_EggPlayer.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

class UEnhancedInputLocalPlayerSubsystem;

ACharacter_EggPlayer::ACharacter_EggPlayer():
PlayerMappingContext(nullptr),
MoveAction(nullptr),
InteractAction(nullptr),
ChangeMoveAxisAction(nullptr),
ReelingAction(nullptr),
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

	FishingRod = AFishingRod::GetFishingRod();

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
	
	if (!CameraBoom)
	{
		CameraBoom = FindComponentByClass<USpringArmComponent>();
	}
}

void ACharacter_EggPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!HandActor || !CameraBoom)
		return;
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
		return;
	
	FVector2D ScreenLocation;
	if (!UGameplayStatics::ProjectWorldToScreen(PC, HandActor->GetActorLocation(), ScreenLocation))
		return;
	
	int32 ViewX, ViewY;
	PC->GetViewportSize(ViewX, ViewY);
	FVector2D ScreenCenter(ViewX * 0.5f, ViewY * 0.5f);
	FVector2D FromCenter = (ScreenLocation - ScreenCenter) / FVector2D(ViewX, ViewY);

	GEngine->AddOnScreenDebugMessage(10, 0.0f, FColor::Green, FString::Printf(
	TEXT("Fron Center: X=%f, Y=%f"), FromCenter.X, FromCenter.Y));
	
	float Distance = FromCenter.Size();
	if (Distance > EdgeThreshold)
	{
		FVector LookDir = (HandActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FRotator TargetRot = UKismetMathLibrary::MakeRotFromX(LookDir);
		FRotator OffsetRot = TargetRot - GetActorRotation();
		
		// Clamp rot
		OffsetRot.Yaw = FMath::Clamp(OffsetRot.Yaw, -MaxCameraYaw, MaxCameraYaw);
		OffsetRot.Pitch = FMath::Clamp(OffsetRot.Pitch, -MaxCameraPitch, MaxCameraPitch);
		OffsetRot.Roll = 0.f;
		
		float EdgeFactor = FMath::Clamp((Distance - EdgeThreshold) / (0.5f - EdgeThreshold), -1.f, 1.f);
		float DynamicSmoothness = FMath::Lerp(LookAtSmoothness, LookAtSmoothness * 4.f, EdgeFactor);
		FRotator NewRot = FMath::RInterpTo(CameraBoom->GetRelativeRotation(), OffsetRot, DeltaTime, DynamicSmoothness);

		CameraBoom->SetRelativeRotation(NewRot);
	}
	else
	{
		// Return to center
		FRotator ResetRot = FMath::RInterpTo(CameraBoom->GetRelativeRotation(),
			FRotator::ZeroRotator, DeltaTime, LookAtSmoothness);
		CameraBoom->SetRelativeRotation(ResetRot);
	}
}


void ACharacter_EggPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction( MoveAction,           ETriggerEvent::Triggered, this, &ACharacter_EggPlayer::Move                    );
		EnhancedInput->BindAction( InteractAction,       ETriggerEvent::Started,   this, &ACharacter_EggPlayer::InteractStarted         );
		EnhancedInput->BindAction( InteractAction,       ETriggerEvent::Completed, this, &ACharacter_EggPlayer::InteractCompleted       );
		EnhancedInput->BindAction( ChangeMoveAxisAction, ETriggerEvent::Started,   this, &ACharacter_EggPlayer::ChangeMoveAxisStarted   );
		EnhancedInput->BindAction( ChangeMoveAxisAction, ETriggerEvent::Completed, this, &ACharacter_EggPlayer::ChangeMoveAxisCompleted );
		EnhancedInput->BindAction( ReelingAction,        ETriggerEvent::Triggered, this, &ACharacter_EggPlayer::ReelingActionTriggered  );
	}
}

void ACharacter_EggPlayer::Move(const FInputActionValue& Value)
{
	if( bInteract )
	{
		auto HitActor = InteractionTrace();
		if( HitActor )
		{
			if( IInteract::Execute_Interact( HitActor, Value.Get<FVector2D>(), HandActor->GetActorLocation() - HandActor->GetActorUpVector() * 10 ) )
				return;
		}
	}
	
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

void ACharacter_EggPlayer::ReelingActionTriggered( const FInputActionValue& Value )
{
	if( FishingRod )
	{
		FishingRod->ReelIn( Value.Get<FVector2D>() );
		return;
	}
	FishingRod = AFishingRod::GetFishingRod();
}

AActor* ACharacter_EggPlayer::InteractionTrace()
{
	TArray<FHitResult> HitResults;
	const FVector Start = HandActor->GetActorLocation();
	const FVector End = Start;
	
	const float Radius = 100.0f;

	TArray< AActor* > ignoreArray = TArray<AActor*>();
	ignoreArray.Add( HandActor );
	ignoreArray.Add( this );

	
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		this,
		Start,
		End,
		Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ignoreArray, // Actors to ignore
		EDrawDebugTrace::ForDuration,
		HitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		0
	);

	for ( const FHitResult& Hit : HitResults )
	{
		AActor* HitActor = Hit.GetActor();
		if ( HitActor && HitActor->GetClass()->ImplementsInterface( UInteract::StaticClass() ) )
		{
			return HitActor;
		}
	}

	//DrawDebugSphere( this->GetWorld(), Start, Radius,
	//	5,
	//	bHit ? FColor::Green : FColor::Red,
	//	false, 0.01f,
	//	0,
	//	5 );

	return nullptr;
}
