// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class GEARFACTORY_API UStatusWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlockEnergyRatio;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBarEnergyRatio;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlockTotalLifes;

protected:
	virtual void NativeConstruct() override;

	bool Initialize() override;

private:
	UFUNCTION()
	FText SetTextBlockEnergyRatio();

	UFUNCTION()
	FText SetTextBlockTotalLifes();

};
