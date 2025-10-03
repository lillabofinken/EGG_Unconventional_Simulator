// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IBoil.generated.h"

// This class does not need to be modified.
UINTERFACE( Blueprintable )
class UIBoil : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EGG_GAMEJAM_API IIBoil
{
	GENERATED_BODY()
	
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = "MyInterface" )
	void Boiling( bool _enter );
};
