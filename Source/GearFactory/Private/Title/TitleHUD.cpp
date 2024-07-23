// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/TitleHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

void ATitleHUD::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("ATitleHUD::BeginPlay()"));
	// WidgetBlueprint��Class���擾����
	FString Path = TEXT("/Game/UI/Title/BPW_Title.BPW_Title_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// PlayerController���擾����
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// WidgetClass��PlayerController���擾�ł��������肷��
	if (WidgetClass && PlayerController)
	{
		// Widget���쐬����
		UUserWidget* UserWidget = UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, PlayerController);

		// Viewport�ɒǉ�����
		UserWidget->AddToViewport(0);

		// MouseCursor��\������
		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
}

