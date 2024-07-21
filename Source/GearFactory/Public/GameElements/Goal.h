// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h" //Particle
#include "Goal.generated.h"

UCLASS()
class GEARFACTORY_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleSystemComponent;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Goal)
	class UStaticMeshComponent* GoalBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Goal)
	class UStaticMeshComponent* GoalArea;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

public:
	//遷移するLevelのソフト参照=必要になった時にアセットをロードし、基本はパスを保持する手法。lazy loading的な
	//今回のパターンでは、主にエディタ上でアセットを選択できるようにするために採用する
	//ゲームインスタンスを使用して、レベル間で遷移してもカメラの向きを保持するようにしたい→遷移するときカメラがガクッとして気持ち悪い
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LevelToLoad")
	TSoftObjectPtr<UWorld> LoadLevel;

};
