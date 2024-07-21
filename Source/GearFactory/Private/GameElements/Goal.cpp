// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/Goal.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //�S�[���̑��ꕔ��
    GoalBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GoalBaseMeshComponent")); //�R���|�[�l���g�ݒ�
    RootComponent = GoalBase;
    UStaticMesh* GoalBaseMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Cylinder"));
    GoalBase->SetStaticMesh(GoalBaseMesh);
    UMaterial* GoalBaseMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/MyAsset/Material/DarkMetal.DarkMetal"));
    GoalBase->SetMaterial(0, GoalBaseMaterial);
    GoalBase->SetRelativeScale3D(FVector(2.0f, 2.0f, 0.1f)); //�T�C�Y����

    //����G���A
    GoalArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GoalAreaMeshComponent")); //�R���|�[�l���g�ݒ�
	GoalArea->SetupAttachment(GoalBase);
    UStaticMesh* GoalAreaMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Cylinder"));
    GoalArea->SetStaticMesh(GoalAreaMesh);
    UMaterial* GoalAreaMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/StarterContent/Materials/M_Glass.M_Glass"));
    GoalArea->SetMaterial(0, GoalAreaMaterial);

    GoalArea->SetCollisionProfileName(TEXT("OverlapAll")); //�ʉ߉\�ɂ���
    GoalArea->SetGenerateOverlapEvents(true);
    GoalArea->SetCollisionResponseToAllChannels(ECR_Overlap);

    GoalArea->SetRelativeLocation(FVector(0.0f, 0.0f, 1.0f)); //�`�󒲐�
    GoalArea->SetRelativeScale3D(FVector(0.5f, 0.5f, 10.0f));

    GoalArea->OnComponentBeginOverlap.AddDynamic(this, &AGoal::OnOverlapBegin); //�C�x���g�ݒ�

    // �p�[�e�B�N��
    ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    ParticleSystemComponent->SetupAttachment(GoalBase);
    ParticleSystemComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Sparks.P_Sparks"));
    if (ParticleAsset.Succeeded())
    {
        ParticleSystemComponent->SetTemplate(ParticleAsset.Object);
    }
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();
	
    //GoalArea���\���ɂ���
    GoalArea->SetVisibility(false);
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGoal::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("Overlap Begin with %s"), *OtherActor->GetName());
        UGameplayStatics::OpenLevelBySoftObjectPtr(this, LoadLevel);
    }
}
