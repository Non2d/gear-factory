// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Gear36.generated.h"

UCLASS()
class GEARFACTORY_API AGear36 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGear36();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* CenterCollision;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
	void OnHit(class UPrimitiveComponent* HitComp, AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	void Rolling(float DeltaTime);

	float GetRelativePlayerRotationYaw(const APawn* PlayerPawnPtr = nullptr);

	int GetPocketLanded(float RelativePlayerYaw);

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Gear;

	// TextRenderComponentを保持するためのポインタ
	UPROPERTY(VisibleAnywhere)
	//UTextRenderComponent* TextRender;
	TArray<UTextRenderComponent*> TextRenderComponents; //可変長配列に変更

};
