// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/RespawnArea.h"
#include "Components/BoxComponent.h"
#include "Framework/PlayingGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARespawnArea::ARespawnArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = DefaultSceneRoot;
	RespawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RespawnArea->SetupAttachment(DefaultSceneRoot);
	RespawnArea->OnComponentBeginOverlap.AddDynamic(this, &ARespawnArea::OnBoxBeginOverlap);
}

// Called when the game starts or when spawned
void ARespawnArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARespawnArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARespawnArea::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ASpherePlayer* Player = Cast<ASpherePlayer>(OtherActor))
	{
		if(APlayingGameMode* GameMode = Cast<APlayingGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			GameMode->KillPlayer(Player); //RespawnPlayerだと、ちゃんと参照エラーではなくinaccessibleと言われる
		}
	}
}

void ARespawnArea::OnConstruction(const FTransform& Transform)
{
	RespawnArea->SetBoxExtent(BoxExtent);
}