// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameElements/ItemBase.h"
#include "ItemEnergy.generated.h"

/**
 * 
 */
UCLASS()
class GEARFACTORY_API AItemEnergy : public AItemBase
{
	GENERATED_BODY()

public:
	AItemEnergy();

	UPROPERTY(EditAnywhere, Category = "Item")
	float ChargeValue = 25.0f;

protected:
	void GetItem() override;

};
