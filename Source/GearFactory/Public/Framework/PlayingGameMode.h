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
	FTransform SpawnTransform; //Player�̃��X�|�[�����W

private:
	void RespawnPlayer();

protected:
	virtual void BeginPlay() override;

public:
	void KillPlayer(ASpherePlayer* Player); //�������KillVolume�I�u�W�F�N�g�ŌĂяo���ARespawn�͖{�N���X���ŌĂяo���B�֐S�̐��ݕ�����private��public�𕪂���I

	void RestartGame();

};
