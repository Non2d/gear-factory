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

    // Gear
    Gear = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GearMeshComponent"));
    RootComponent = Gear;
    UStaticMesh* GearMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MyAsset/Shapes/Shape_Gear36"));
    Gear->SetStaticMesh(GearMesh);
    Gear->SetRelativeScale3D(FVector(8.2f, 8.2f, 1.0f));

    UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Game/MyAsset/Material/DarkMetal.DarkMetal"));
    Gear->SetMaterial(0, Material);

    Gear->BodyInstance.bNotifyRigidBodyCollision = true;

    // CenterCollision
    CenterCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CenterCollisionComponent"));
    CenterCollision->AttachToComponent(Gear, FAttachmentTransformRules::KeepRelativeTransform);    // CenterCollisionをGearの子コンポーネントとしてアタッチ

    UStaticMesh* CenterCollisionMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Cylinder")); //static meshとmaterialの設定
    CenterCollision->SetStaticMesh(CenterCollisionMesh);
    UMaterial* CenterCollisionMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/StarterContent/Materials/M_Glass.M_Glass"));
    CenterCollision->SetMaterial(0, CenterCollisionMaterial);

    CenterCollision->SetRelativeScale3D(FVector(1.4f, 1.4f, 0.3f)); // CenterCollisionの相対スケールを設定してZ軸方向の大きさを調整
    CenterCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));

    CenterCollision->SetCollisionProfileName(TEXT("OverlapAll")); // Collisionって言ってるけど衝突は消します(Areaとかのほうが良かったね)
    CenterCollision->SetGenerateOverlapEvents(true);
    CenterCollision->SetCollisionResponseToAllChannels(ECR_Overlap);

    CenterCollision->OnComponentBeginOverlap.AddDynamic(this, &AGear36::OnOverlapBegin); // デリゲートのバインド
    CenterCollision->OnComponentHit.AddDynamic(this, &AGear36::OnHit);

    // Text Render Component
    //TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent")); // Gearの子供
    //TextRender->SetupAttachment(Gear);

    //float angle = FMath::DegreesToRadians(5.0f);
    //TextRender->SetRelativeScale3D(FVector(1/8.2f, 1/8.2f, 1/1.0f));
    //TextRender->SetText(FText::FromString("1"));
    //TextRender->SetHorizontalAlignment(EHTA_Center);
    //TextRender->SetWorldSize(100.0f); // テキストのサイズ
    //TextRender->SetRelativeLocation(FVector(60.0f*cos(angle), 60.0f*sin(angle), 100.0f)); // テキストの位置
    //TextRender->SetRelativeRotation(FRotator(30.0f, 0.0f, 0.0f)); // テキストの回転

    for (int32 i = 1; i <= 18; i++) // 0度から350度まで、20度ごとに18個のテキストを配置
    {
        float angleDegree = 20.0f*(i-1) + 5.0f;
        float angle = FMath::DegreesToRadians(angleDegree);
        FString ComponentName = FString::Printf(TEXT("TextRenderComponent_%d"), i);
        UTextRenderComponent* TextRender = CreateDefaultSubobject<UTextRenderComponent>(*ComponentName);

        TextRender->AttachToComponent(Gear, FAttachmentTransformRules::KeepRelativeTransform);
        TextRender->SetRelativeScale3D(FVector(1 / 8.2f, 1 / 8.2f, 1 / 1.0f));
        TextRender->SetText(FText::FromString(FString::FromInt(i)));
        TextRender->SetHorizontalAlignment(EHTA_Center);
        TextRender->SetWorldSize(100.0f);
        TextRender->SetRelativeLocation(FVector(60.0f * FMath::Cos(angle), 60.0f * FMath::Sin(angle), 100.0f));
        TextRender->SetRelativeRotation(FRotator(30.0f, angleDegree, 0.0f));

        TextRenderComponents.Add(TextRender); // 配列に追加
    }
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

    //float RelativeYaw = AGear36::GetRelativePlayerRotationYaw();
    //UE_LOG(LogTemp, Warning, TEXT("RelativeYaw: %f"), RelativeYaw);
    //int PocketLanded = AGear36::GetPocketLanded(RelativeYaw);
    //UE_LOG(LogTemp, Warning, TEXT("PocketLanded: %d"), PocketLanded);
}

void AGear36::Rolling(float DeltaTime)
{
	// 回転速度（度/秒）
    float RotationSpeed = -10.0f;

    // DeltaTimeを使用して回転角度を計算
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += RotationSpeed * DeltaTime;

    // 新しい回転角度を適用
    SetActorRotation(NewRotation);
}

float AGear36::GetRelativePlayerRotationYaw(const APawn* PlayerPawnPtr) { //規定値はヘッダでのみ定義し、cppでは定義しないと。
    // UEでは、FVector*ではなくオブジェクトのポインタを使うことが多いのであとで定義ごと修正
    //Playerの座標を取得
    FVector PlayerLocation;
    if (PlayerPawnPtr) {
        // 引数が提供された場合、その値を使用
        PlayerLocation = PlayerPawnPtr->GetActorLocation();
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

int AGear36::GetPocketLanded(float RelativePlayerYaw) {
    if (RelativePlayerYaw <= 0 || RelativePlayerYaw > 360.0f) {
        UE_LOG(LogTemp, Warning, TEXT("Error: RelativePlayerYaw is out of range"));
        return -1;
    }
    int pocketNumber = static_cast<int>(RelativePlayerYaw / 20.0f) + 1;
    return pocketNumber;
}

void AGear36::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        // OtherActorがAPawnクラスのインスタンスであるかチェック
        APawn* PlayerPawnPtr = Cast<APawn>(OtherActor);
        if (PlayerPawnPtr)
        {
            float RelativeYaw = AGear36::GetRelativePlayerRotationYaw(PlayerPawnPtr);
            UE_LOG(LogTemp, Warning, TEXT("RelativeYaw: %f"), RelativeYaw);
            int PocketLanded = AGear36::GetPocketLanded(RelativeYaw);
            UE_LOG(LogTemp, Warning, TEXT("PocketLanded: %d"), PocketLanded);
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