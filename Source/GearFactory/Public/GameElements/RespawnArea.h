// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnArea.generated.h"

class UBoxComponent;

UCLASS()
class GEARFACTORY_API ARespawnArea : public AActor //Sub Level�ɔz�u����ꍇ�́AAlwaysLoaded�ɂ��Ȃ��ƑʖځI
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARespawnArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere) //meta=(AllowPrivateAccess="true")��BP������A�N�Z�X�\�ɂȂ�
	TObjectPtr<UBoxComponent> RespawnArea;

protected:
	virtual void OnConstruction(const FTransform& Transform) override; //BP�ł���ConstructionScript�B�r���[�ɂ����Ă���T�C�Y�𒲐��ł���I

public:
	UPROPERTY(EditAnywhere)
	FVector BoxExtent = FVector(32.0f, 32.0f, 32.0f);

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
