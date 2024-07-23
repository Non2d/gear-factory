// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Playable/SpherePlayer.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = ItemMesh;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(50.0f);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnSphereBeginOverlap);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	RotatingMovement->SetUpdatedComponent(ItemMesh);

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::GetItem()
{
	//�����͎q�N���X�Ŏ���
}

void AItemBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �ڐG����Actor��BallPlayer�����肷��
	if (const ASpherePlayer* Player = Cast<ASpherePlayer>(OtherActor))
	{
		// Item���擾���鏈������������
		this->GetItem();

		// �A�C�e����j������
		this->Destroy();
	}
}