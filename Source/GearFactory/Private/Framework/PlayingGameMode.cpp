// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PlayingGameMode.h"
#include "Playable/SpherePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

APlayingGameMode::APlayingGameMode()
{
    DefaultPawnClass = ASpherePlayer::StaticClass();  //Playerのクラスを設定
}

void APlayingGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Viewportに配置されたPlayerStartを探す
	const APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));

	// PlayerStartの位置情報をRespornの位置として保持する
	SpawnTransform = PlayerStart->GetActorTransform();
}

void APlayingGameMode::RespawnPlayer()
{
	// BallPlayerをSpawnする
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ASpherePlayer* Player = GetWorld()->SpawnActor<ASpherePlayer>(ASpherePlayer::StaticClass(), SpawnTransform, SpawnInfo);

	// Controllerを設定する
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->Possess(Player);
}

void APlayingGameMode::KillPlayer(ASpherePlayer* Player) //こちらを外部でアクセス可能に
{
	Player->Destroy();
	RespawnPlayer();
}