// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Playable/SpherePlayer.h"
#include "PlayingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GEARFACTORY_API APlayingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APlayingGameMode();

public:
	FTransform SpawnTransform; //Playerのリスポーン座標

private:
	void RespawnPlayer();

protected:
	virtual void BeginPlay() override;

public:
	void KillPlayer(ASpherePlayer* Player); //こちらはKillVolumeオブジェクトで呼び出し、Respawnは本クラス内で呼び出す。関心の棲み分けでprivateとpublicを分ける！

	void RestartGame();

};
