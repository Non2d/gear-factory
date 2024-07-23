// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GearFactoryGameInstance.h"

void UGearFactoryGameInstance::Initialize()
{
	const UGearFactoryGameInstance* DefaultObject = GetDefault<UGearFactoryGameInstance>();
	this->TotalLifes = DefaultObject->TotalLifes;
}