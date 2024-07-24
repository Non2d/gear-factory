// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatusWidget.h"
#include "Components/TextBlock.h"
#include "Playable/SpherePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/GearFactoryGameInstance.h"
#include "Components/ProgressBar.h"

void UStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UStatusWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	TextBlockEnergyRatio->TextDelegate.BindUFunction(this, "SetTextBlockEnergyRatio");  // コレをコメントアウトしビルドしておかないとBPWは作れない
	TextBlockTotalLifes->TextDelegate.BindUFunction(this, "SetTextBlockTotalLifes");    //BPW(StatusWidgetの子)を保存すると参照エラーで落ちる

	return true;
}

FText UStatusWidget::SetTextBlockEnergyRatio()
{
	//if (const ASpherePlayer* Player = Cast<ASpherePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	//{
	//	// Assuming you want to display the ratio of current energy to max energy as a percentage
	//	float EnergyRatio = (Player->GetEnergy() / Player->GetEnergyMax()) * 100;
	//	return FText::FromString(FString::Printf(TEXT("%.0f%%"), EnergyRatio));
	//}

	return FText();
}


FText UStatusWidget::SetTextBlockTotalLifes()
{
	if (const UGearFactoryGameInstance* GameInstance = Cast<UGearFactoryGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		return FText::FromString(FString::FromInt(GameInstance->TotalLifes));
	}
	return FText();
}