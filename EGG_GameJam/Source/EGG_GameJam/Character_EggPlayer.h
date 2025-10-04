// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "FishingRod.h"

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
	void InteractStarted(const FInputActionValue& Value);
	void InteractCompleted(const FInputActionValue& Value);
	void ChangeMoveAxisStarted(const FInputActionValue& Value);
	void ChangeMoveAxisCompleted(const FInputActionValue& Value);
	void ReelingActionTriggered(const FInputActionValue& Value);
 
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* InteractAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ChangeMoveAxisAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ReelingAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float CameraPanStrength = 10.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float CameraPanSmoothing = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
    float EdgeThreshold = 0.35f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
    float LookAtStrength = 0.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
    float LookAtSmoothness = 4.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float MaxCameraYaw = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float MaxCameraPitch = 8.0f;

	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> HandBP;

	UPROPERTY(BlueprintReadOnly)
	class AActor* HandActor;
	
	UPROPERTY(BlueprintReadOnly)
	USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadOnly)
	bool bInteract;
	UPROPERTY(BlueprintReadOnly)
	bool bDepthMode;
	
	float MoveSpeed;

private:
	UPROPERTY()
	AFishingRod* FishingRod = nullptr;
};
