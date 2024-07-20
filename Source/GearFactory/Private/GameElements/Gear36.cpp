// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/Gear36.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h" // APawnクラスを使用するために必要

// Sets default values
AGear36::AGear36()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create and attach the static mesh component
    Gear = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GearMeshComponent"));
    RootComponent = Gear;
    UStaticMesh* GearMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MyAsset/Shapes/Shape_Gear36"));
    Gear->SetStaticMesh(GearMesh);
    Gear->SetRelativeScale3D(FVector(8.2f, 8.2f, 1.0f));

    UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Polished.M_Wood_Floor_Walnut_Polished"));
    Gear->SetMaterial(0, Material);

    Gear->BodyInstance.bNotifyRigidBodyCollision = true;

    // スタティックメッシュコンポーネントの作成
    CenterCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CenterCollisionComponent"));
    // CenterCollisionをGearの子コンポーネントとしてアタッチ
    CenterCollision->AttachToComponent(Gear, FAttachmentTransformRules::KeepRelativeTransform);

    // スタティックメッシュの設定
    UStaticMesh* CenterCollisionMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Cylinder"));
    CenterCollision->SetStaticMesh(CenterCollisionMesh);

    // CenterCollisionの相対スケールを設定してZ軸方向の大きさを調整
    CenterCollision->SetRelativeScale3D(FVector(1.4f, 1.4f, 0.3f));
    CenterCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));

    // コリジョン設定の変更
    CenterCollision->SetCollisionProfileName(TEXT("OverlapAll"));
    CenterCollision->SetGenerateOverlapEvents(true);
    CenterCollision->SetCollisionResponseToAllChannels(ECR_Overlap);

    // 物理シミュレーションの無効化
    CenterCollision->SetSimulatePhysics(false);

    CenterCollision->OnComponentBeginOverlap.AddDynamic(this, &AGear36::OnOverlapBegin);
    CenterCollision->OnComponentHit.AddDynamic(this, &AGear36::OnHit);

}

// Called when the game starts or when spawned
void AGear36::BeginPlay()
{
	Super::BeginPlay();
	
    // ゲームプレイ時には非表示にする&オーバーラップイベントは有効のまま。本当はかっこよく#if !WITH_EDITORでやりたいけど、なぜかうまくいかないのでこのままで。コンポーネント多いと初期化処理重くなるので注意
    // やっぱり、コンパイルの真価は起動時の処理工程をなるたけ削る点にあるんよね
    CenterCollision->SetVisibility(false, true); // CenterCollisionの子コンポーネントも非表示にする
}

// Called every frame
void AGear36::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Rolling(DeltaTime);

    //Yawだけ取り出す
    //UE_LOG(LogTemp, Warning, TEXT("RelativeYaw: %f"), AGear36::CalcRelativePlayerRotationYaw());
}

void AGear36::Rolling(float DeltaTime)
{
	// 回転速度（度/秒）
    float RotationSpeed = 10.0f;

    // DeltaTimeを使用して回転角度を計算
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += RotationSpeed * DeltaTime;

    // 新しい回転角度を適用
    SetActorRotation(NewRotation);
}

float AGear36::CalcRelativePlayerRotationYaw(const FVector* PlayerLocationPtr) { //規定値はヘッダでのみ定義し、cppでは定義しないと。
    // UEでは、FVector*ではなくオブジェクトのポインタを使うことが多いのであとで定義ごと修正
    //Playerの座標を取得
    FVector PlayerLocation;
    if (PlayerLocationPtr) {
        // 引数が提供された場合、その値を使用
        PlayerLocation = *PlayerLocationPtr;
    } else {
        // 引数が提供されなかった場合、現在のプレイヤーの座標を取得
        PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
    }

    FVector GearToPlayer = PlayerLocation - GetActorLocation();
    FRotator RotationGearToPlayer = GearToPlayer.Rotation();
    FRotator RelativePlayerRotation = RotationGearToPlayer - GetActorRotation();
    float RelativePlayerRotationYaw = RelativePlayerRotation.Yaw;
    if (RelativePlayerRotationYaw<0) {
        RelativePlayerRotationYaw += 360.0f;
    }

    return  RelativePlayerRotationYaw;
}

void AGear36::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        // OtherActorがAPawnクラスのインスタンスであるかチェック
        APawn* PlayerPawn = Cast<APawn>(OtherActor);
        if (PlayerPawn)
        {
            FVector* PlayerLocationPtr = new FVector(PlayerPawn->GetActorLocation());
            UE_LOG(LogTemp, Warning, TEXT("RelativeYaw: %f"), AGear36::CalcRelativePlayerRotationYaw(PlayerLocationPtr));
        }
    }
}

void AGear36::OnHit(class UPrimitiveComponent* HitComp, AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // 多分使わないけど
    if (OtherActor && (OtherActor != this) && OtherComp){
        UE_LOG(LogTemp, Warning, TEXT("Hit with %s"), *OtherActor->GetName());
    }
}