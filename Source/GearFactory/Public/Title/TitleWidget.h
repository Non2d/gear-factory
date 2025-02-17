// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class GEARFACTORY_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// NativeConstruct
	void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonPlay;

	// ButtonPlayのOnClickedイベントに関連づける
	UFUNCTION()
	void OnButtonPlayClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonQuit;

	// ButtonQuitのOnClickedイベントに関連づける
	UFUNCTION()
	void OnButtonQuitClicked();

};
