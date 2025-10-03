// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character_EggPlayer.generated.h"

UCLASS()
class EGG_GAMEJAM_API ACharacter_EggPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacter_EggPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerMappingContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* CarMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* LookAction;
};
