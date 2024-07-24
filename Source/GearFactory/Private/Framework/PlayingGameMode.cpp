// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PlayingGameMode.h"
#include "Playable/SpherePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Framework/GearFactoryGameInstance.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"


APlayingGameMode::APlayingGameMode()
{
    DefaultPawnClass = ASpherePlayer::StaticClass();  //Playerのクラスを設定

	//HUDの設定(BPから輸入)
	static ConstructorHelpers::FClassFinder<AHUD> HUDClassFinder(TEXT("/Game/UI/Playing/HUD_Playing.HUD_Playing_C"));
	if (HUDClassFinder.Succeeded())
	{
		HUDClass = HUDClassFinder.Class;
	}

	//PlayerControllerの設定(BPから輸入)
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassFinder(TEXT("/Game/UI/Playing/PC_Playing.PC_Playing_C"));
	if (PlayerControllerClassFinder.Succeeded())
	{
		PlayerControllerClass = PlayerControllerClassFinder.Class;
	}
}

void APlayingGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Viewportに配置されたPlayerStartを探す
	const APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));

	// PlayerStartの位置情報をRespawnの位置として保持する
	SpawnTransform = PlayerStart->GetActorTransform();

	// 長押ししなくてもマウスで視点操作できるように！！！！
	/*APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;*/

	SetInitialEnergyBasedOnLevelName();
}

void APlayingGameMode::SetInitialEnergyBasedOnLevelName()
{
	// レベルごとの体力設定を適用
	ASpherePlayer* Player = Cast<ASpherePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	FName CurrentLevelName = *UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	UE_LOG(LogTemp, Warning, TEXT("CurrentLevelName: %s"), *CurrentLevelName.ToString());

	if (CurrentLevelName == "Level02" || CurrentLevelName=="Level03") {
		Player->SetEnergy(10); // SetHealth は ASpherePlayer で体力を設定する関数
	}
	else if (CurrentLevelName == "TestLevel") {
		Player->SetEnergy(90);
	}
	else {
		Player->SetEnergy(100);
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
	// これまでは残機-1で強制的にリセット&再開だったが、今後はまずGame Over画面を表示して、そこでリトライ(=リセット&再開)かタイトルに戻るかを選択できるようにする
	//// GameInstanceの初期化
	//UGearFactoryGameInstance* GameInstance = Cast<UGearFactoryGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//GameInstance->Initialize();

	////// レベルを開き直す
	//FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	//UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName));

	// Game Overレベルに遷移
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("GameOver")));
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

	// Energyを初期値に
	SetInitialEnergyBasedOnLevelName();
}

