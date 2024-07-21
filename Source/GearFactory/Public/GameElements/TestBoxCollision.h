// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Particles/ParticleSystemComponent.h" //Particle
#include "TestBoxCollision.generated.h"

UCLASS()
class GEARFACTORY_API ATestBoxCollision : public AActor
{
	//Boxじゃなくて円柱だけどテストなので許して
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestBoxCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* StaticMeshComponent;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(class UPrimitiveComponent* HitComp, AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// TextRenderComponentを保持するためのポインタ
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* TextRender;

protected:
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleSystemComponent;

};
