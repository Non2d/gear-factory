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
	// GameInstance�̕ϐ�������������...�͂�

	// Level01��Load����
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Level01")));
}

void UTitleWidget::OnButtonQuitClicked()
{
	// PlayerController���擾����
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// �Q�[�����I������
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}