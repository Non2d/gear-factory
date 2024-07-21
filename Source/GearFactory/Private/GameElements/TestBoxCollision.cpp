#include "GameElements/TestBoxCollision.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

// Sets default values
ATestBoxCollision::ATestBoxCollision()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // スタティックメッシュコンポーネントの作成
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    RootComponent = StaticMeshComponent;

    // スタティックメッシュの設定
    UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Cylinder"));
    StaticMeshComponent->SetStaticMesh(Mesh);

    // コリジョン設定の変更
    StaticMeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    StaticMeshComponent->SetGenerateOverlapEvents(true);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

    // 物理シミュレーションの無効化
    StaticMeshComponent->SetSimulatePhysics(false);

    StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ATestBoxCollision::OnOverlapBegin);
    StaticMeshComponent->OnComponentHit.AddDynamic(this, &ATestBoxCollision::OnHit);

    // Text Render Component
    // TextRenderComponentのインスタンスを作成し、ルートコンポーネントとして設定
    TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
    TextRender->SetupAttachment(StaticMeshComponent);

    // テキストの内容とその他のプロパティを設定
    TextRender->SetText(FText::FromString("Hello, Unreal!"));
    TextRender->SetHorizontalAlignment(EHTA_Center);
    TextRender->SetWorldSize(100.0f); // テキストのサイズ
    TextRender->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // テキストの位置
    TextRender->SetRelativeRotation(FRotator(45.0f, 0.0f, 0.0f)); // テキストの回転

    // パーティクル
    ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    ParticleSystemComponent->SetupAttachment(StaticMeshComponent);
    ParticleSystemComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Sparks.P_Sparks"));
    if (ParticleAsset.Succeeded())
    {
        ParticleSystemComponent->SetTemplate(ParticleAsset.Object);
    }

}

// Called when the game starts or when spawned
void ATestBoxCollision::BeginPlay()
{
    Super::BeginPlay();

    // ゲームプレイ時には非表示にする（オーバーラップイベントは有効）
    //StaticMeshComponent->SetVisibility(false, true); // 子コンポーネントも非表示にする
}

// Called every frame
void ATestBoxCollision::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATestBoxCollision::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("Overlap Begin with %s"), *OtherActor->GetName());
    }
}

void ATestBoxCollision::OnHit(class UPrimitiveComponent* HitComp, AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit with %s"), *OtherActor->GetName());
    }
}
