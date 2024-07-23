#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"     // Enhanced Inputを使用
#include "InputMappingContext.h"  // UInputMappingContextのヘッダーを追加

#include "SpherePlayer.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

class UInputMappingContext;       // Enhanced Inputを使用
class UInputAction;               // Enhanced Inputを使用

class UArrowComponent;  // デバッグ用Arrow

UCLASS()
class GEARFACTORY_API ASpherePlayer : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    ASpherePlayer();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyHit(
        class UPrimitiveComponent* MyComp,
        class AActor* Other,
        class UPrimitiveComponent* OtherComp,
        bool bSelfMoved,
        FVector HitLocation,
        FVector HitNormal,
        FVector NormalImpulse,
        const FHitResult& Hit
    ) override;

private:
    UPROPERTY(VisibleAnywhere, Category=Playable, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UStaticMeshComponent> Sphere;

    UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> SpringArm;

    UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ControlAction;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CameraAction;

    UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> BoostAction;


private:
    float Speed = 500.0f;
    float Energy = 100.0f;
    float EnergyMax = 100.0f;
	int32 JumpCount = 2;
	const int32 MaxJumpCount = 2; // 最大ジャンプ回数を2に設定
    float Torque = 500000000.0f;

protected:
    void ControlPlayer(const FInputActionValue& Value);
	
	// virtual void Landed(const FHitResult& Hit) override; //Characterクラスじゃないとだめだった!! Hitよりも着地判定になってるっぽい
	void JumpPlayer(const FInputActionValue& Value);

	void ControlCamera(const FInputActionValue& Value);

    void BoostPlayer(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere, Category = Control, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> Arrow;

public:
    float SetEnergy(const float NewEnergy);
    float ChargeEnergy(const float ChargeValue);
    float ConsumeEnergy(const float ConsumeValue);

};
