// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GearFactoryGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GEARFACTORY_API UGearFactoryGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	int32 TotalLifes = 1;

	UFUNCTION(BlueprintCallable)
	void Initialize();
	
};
