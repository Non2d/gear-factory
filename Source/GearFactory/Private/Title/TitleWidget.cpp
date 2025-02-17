// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/TitleWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonPlay->OnClicked.AddUniqueDynamic(this, &UTitleWidget::OnButtonPlayClicked);
	ButtonQuit->OnClicked.AddUniqueDynamic(this, &UTitleWidget::OnButtonQuitClicked);
}

void UTitleWidget::OnButtonPlayClicked()
{
	// GameInstanceの変数を初期化する...はず

	// Level01をLoadする
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Level01")));
}

void UTitleWidget::OnButtonQuitClicked()
{
	// PlayerControllerを取得する
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// ゲームを終了する
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}