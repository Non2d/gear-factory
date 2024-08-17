// Fill out your copyright notice in the Description page of Project Settings.

#include "Playable/SpherePlayer.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"	 // Enhanced Inputを使用
#include "EnhancedInputSubsystems.h" // Enhanced Inputを使用

#include "Kismet/GameplayStatics.h"

#include "Components/ArrowComponent.h"

#include "Framework/PlayingGameMode.h" //KillPlayer関数を呼び出すために必要

// Sets default values
ASpherePlayer::ASpherePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// プレイヤーのスフィアを作成
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = Sphere;

	UStaticMesh *Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));

	Sphere->SetStaticMesh(Mesh);

	UMaterial *Material = LoadObject<UMaterial>(nullptr, TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse"));

	Sphere->SetMaterial(0, Material);

	Sphere->SetSimulatePhysics(true);

	Sphere->BodyInstance.bNotifyRigidBodyCollision = true;

	// カメラの制御
	//  Arm追加
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 450.0f;

	// PawnのControllerRotationを使用する
	SpringArm->bUsePawnControlRotation = true;

	// CameraのLegを有効にする
	SpringArm->bEnableCameraLag = true;

	// Camera追加
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm);

	// Motion Blurを有効にする
	Camera->PostProcessSettings.MotionBlurAmount = 0.0f;

	// ジャンプ関連
	JumpCount = MaxJumpCount;

	// Inputs
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Input/IM_Controls"));
	ControlAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Action/IA_Control"));
	JumpAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Action/IA_Jump"));
	CameraAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Action/IA_Look"));
	BoostAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Action/IA_Boost"));

	// Arrowを追加する
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	Arrow->SetupAttachment(Camera);

	// Sphereの頭上に移動するようにLocationを設定する
	Arrow->SetRelativeLocation(FVector(400.0f, 0.0f, 130.0f));

	// Arrowを表示されるようにする
	// Arrow->bHiddenInGame = false;
}

// Called when the game starts or when spawned
void ASpherePlayer::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController *PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASpherePlayer::ControlPlayer(const FInputActionValue &Value)
{
	const FVector2D V = Value.Get<FVector2D>(); //From Input value to force toward player sphere
	FVector ForceVector = FVector(V.Y, V.X, 0.0f) * Speed;
	FVector ArrowForceVector = Arrow->GetComponentToWorld().TransformVectorNoScale(ForceVector);
	Sphere->AddForce(ArrowForceVector, TEXT("NONE"), true);

	ConsumeEnergy(ArrowForceVector.Size() * 0.00001f);
}

void ASpherePlayer::BoostPlayer(const FInputActionValue &Value)
{
	// InputのValueはbooleanで、trueのときだけ前方向に急回転させる
	if (Value.Get<bool>())
	{
		FVector ForwardVector = Arrow->GetForwardVector().GetSafeNormal(0.0001f); //Arrow前方方向に進行させる
		FVector TorqueVector = FVector(ForwardVector.Y * Torque * -1.0f, ForwardVector.X * Torque, 0.0f);
		Sphere->AddTorqueInRadians(TorqueVector, TEXT("None"), true);
		ConsumeEnergy(0.05f);
	}
}

void ASpherePlayer::JumpPlayer(const FInputActionValue &Value)
{
	// JumpCountを表示
	if (Value.Get<bool>() && JumpCount > 0)
	{
		Sphere->AddImpulse(FVector(0.0f, 0.0f, 500.0f), NAME_None, true);
		JumpCount--;

		ConsumeEnergy(10.0f);
	}
}

void ASpherePlayer::ControlCamera(const FInputActionValue &Value)
{
	// inputのValueはVector2Dに変換できる(ようにInput ActionのValue Typeで設定してある)
	const FVector2D V = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		FRotator ControlRotate = GetControlRotation();
		double LimitPitchAngle = FMath::ClampAngle(ControlRotate.Pitch + V.Y, -40.0f, -10.0f);
		FRotator NewControlRotation = FRotator(LimitPitchAngle, ControlRotate.Yaw + V.X, 0.0f);

		// SetControlRotationの引数には>>1F後の値<<を入れるイメージ
		UGameplayStatics::GetPlayerController(this, 0)->SetControlRotation(NewControlRotation); //AddControlROtationの代替
	}
}

// Called to bind functionality to input
void ASpherePlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ControlAction, ETriggerEvent::Triggered, this, &ASpherePlayer::ControlPlayer);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASpherePlayer::JumpPlayer);
		EnhancedInputComponent->BindAction(CameraAction, ETriggerEvent::Triggered, this, &ASpherePlayer::ControlCamera);
		EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Triggered, this, &ASpherePlayer::BoostPlayer);
	}
}

void ASpherePlayer::NotifyHit(
	UPrimitiveComponent *MyComp,
	AActor *Other,
	UPrimitiveComponent *OtherComp,
	bool bSelfMoved,
	FVector HitLocation,
	FVector HitNormal,
	FVector NormalImpulse,
	const FHitResult &Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Reset jump count when landing
	JumpCount = MaxJumpCount;
}

float ASpherePlayer::SetEnergy(const float NewEnergy) //基本的にEnergyの初期値をレベルごとに設定する
{
	Energy = FMath::Clamp(NewEnergy, 0.0f, EnergyMax);
	return Energy;
}

float ASpherePlayer::Charge

(const float ChargeValue)
{
	Energy = FMath::Clamp(Energy + ChargeValue, 0.0f, EnergyMax);
	return Energy;
}

float ASpherePlayer::ConsumeEnergy(const float ConsumeValue)
{
	Energy = FMath::Clamp(Energy - ConsumeValue, 0.0f, EnergyMax);
	if (Energy <= 0)
	{
		if (APlayingGameMode* GameMode = Cast<APlayingGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			GameMode->KillPlayer(this);
		}
	}
	return Energy;
}