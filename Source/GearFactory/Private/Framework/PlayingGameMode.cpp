// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PlayingGameMode.h"
#include "Playable/SpherePlayer.h"

APlayingGameMode::APlayingGameMode()
{
    //default pawnを設定
    DefaultPawnClass = ASpherePlayer::StaticClass();
}