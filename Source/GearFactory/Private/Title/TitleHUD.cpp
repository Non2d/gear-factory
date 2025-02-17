// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/TitleHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

void ATitleHUD::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("ATitleHUD::BeginPlay()"));
	// WidgetBlueprintのClassを取得する
	FString Path = TEXT("/Game/UI/Title/BPW_Title.BPW_Title_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// PlayerControllerを取得する
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// WidgetClassとPlayerControllerが取得できたか判定する
	if (WidgetClass && PlayerController)
	{
		// Widgetを作成する
		UUserWidget* UserWidget = UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, PlayerController);

		// Viewportに追加する
		UserWidget->AddToViewport(0);

		// MouseCursorを表示する
		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
}

