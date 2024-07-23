// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PlayingGameMode.h"
#include "Playable/SpherePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Framework/GearFactoryGameInstance.h"

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

	// 長押ししなくてもマウスで視点操作できるように！！！！
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;

	// レベルごとの体力設定を適用
	ASpherePlayer* Player = Cast<ASpherePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	FName CurrentLevelName = *UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	UE_LOG(LogTemp, Warning, TEXT("CurrentLevelName: %s"), *CurrentLevelName.ToString());
	
	if (CurrentLevelName == "Level01") {
		Player->SetEnergy(20); // SetHealth は ASpherePlayer で体力を設定する関数
	}
	else if (CurrentLevelName == "TestLevel") {
		Player->SetEnergy(10);
	}
	else {
		Player->SetEnergy(90);
	}
}

void APlayingGameMode::KillPlayer(ASpherePlayer* Player) //こちらを外部でアクセス可能に
{
	Player->Destroy();

	//Project SettingsでGear FactroryのGame Instanceを指定しろ！！！特にエラーとかでなかったから気づきにくいけど、これがないとGameInstanceの取得に失敗する！！！->TotalLifesもnullptrになって落ちる
	UGearFactoryGameInstance* GameInstance = Cast<UGearFactoryGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->TotalLifes--;

	if (0 <= GameInstance->TotalLifes)
	{
		UE_LOG(LogTemp, Display, TEXT("TotalLifes: %d"), GameInstance->TotalLifes);
		RespawnPlayer();
	}
	else
	{
		// GameをRestartする
		UE_LOG(LogTemp, Display, TEXT("GameOver"));
		RestartGame();
	}
}

void APlayingGameMode::RestartGame()
{
	// GameInstanceの初期化
	UGearFactoryGameInstance* GameInstance = Cast<UGearFactoryGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->Initialize();

	// レベルを開き直す
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName));
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

