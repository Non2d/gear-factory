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
	//�J�ڂ���Level�̃\�t�g�Q��=�K�v�ɂȂ������ɃA�Z�b�g�����[�h���A��{�̓p�X��ێ������@�Blazy loading�I��
	//����̃p�^�[���ł́A��ɃG�f�B�^��ŃA�Z�b�g��I���ł���悤�ɂ��邽�߂ɍ̗p����
	//�Q�[���C���X�^���X���g�p���āA���x���ԂőJ�ڂ��Ă��J�����̌�����ێ�����悤�ɂ��������J�ڂ���Ƃ��J�������K�N�b�Ƃ��ċC��������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LevelToLoad")
	TSoftObjectPtr<UWorld> LoadLevel;

};
