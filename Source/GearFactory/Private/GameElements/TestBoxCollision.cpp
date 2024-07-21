#include "GameElements/TestBoxCollision.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

// Sets default values
ATestBoxCollision::ATestBoxCollision()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // �X�^�e�B�b�N���b�V���R���|�[�l���g�̍쐬
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    RootComponent = StaticMeshComponent;

    // �X�^�e�B�b�N���b�V���̐ݒ�
    UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Cylinder"));
    StaticMeshComponent->SetStaticMesh(Mesh);

    // �R���W�����ݒ�̕ύX
    StaticMeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    StaticMeshComponent->SetGenerateOverlapEvents(true);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

    // �����V�~�����[�V�����̖�����
    StaticMeshComponent->SetSimulatePhysics(false);

    StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ATestBoxCollision::OnOverlapBegin);
    StaticMeshComponent->OnComponentHit.AddDynamic(this, &ATestBoxCollision::OnHit);

    // Text Render Component
    // TextRenderComponent�̃C���X�^���X���쐬���A���[�g�R���|�[�l���g�Ƃ��Đݒ�
    TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
    TextRender->SetupAttachment(StaticMeshComponent);

    // �e�L�X�g�̓��e�Ƃ��̑��̃v���p�e�B��ݒ�
    TextRender->SetText(FText::FromString("Hello, Unreal!"));
    TextRender->SetHorizontalAlignment(EHTA_Center);
    TextRender->SetWorldSize(100.0f); // �e�L�X�g�̃T�C�Y
    TextRender->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // �e�L�X�g�̈ʒu
    TextRender->SetRelativeRotation(FRotator(45.0f, 0.0f, 0.0f)); // �e�L�X�g�̉�]

    // �p�[�e�B�N��
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

    // �Q�[���v���C���ɂ͔�\���ɂ���i�I�[�o�[���b�v�C�x���g�͗L���j
    //StaticMeshComponent->SetVisibility(false, true); // �q�R���|�[�l���g����\���ɂ���
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
