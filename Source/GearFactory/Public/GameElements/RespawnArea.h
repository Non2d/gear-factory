// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnArea.generated.h"

class UBoxComponent;

UCLASS()
class GEARFACTORY_API ARespawnArea : public AActor //Sub Levelに配置する場合は、AlwaysLoadedにしないと駄目！
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

	UPROPERTY(VisibleAnywhere) //meta=(AllowPrivateAccess="true")でBPからもアクセス可能になる
	TObjectPtr<UBoxComponent> RespawnArea;

protected:
	virtual void OnConstruction(const FTransform& Transform) override; //BPでいうConstructionScript。ビューにおいてからサイズを調整できる！

public:
	UPROPERTY(EditAnywhere)
	FVector BoxExtent = FVector(32.0f, 32.0f, 32.0f);

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
