// Fill out your copyright notice in the Description page of Project Settings.

#include "GameElements/ItemEnergy.h"
#include "Playable/SpherePlayer.h"
#include "Kismet/GameplayStatics.h"

AItemEnergy::AItemEnergy()
{
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(NULL, TEXT("/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule"), NULL, LOAD_None, NULL);
	ItemMesh->SetStaticMesh(Mesh);
	ItemMesh->SetMaterial(0, LoadObject<UMaterial>(nullptr, TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile.M_Tech_Hex_Tile")));
}

void AItemEnergy::GetItem()
{
	ASpherePlayer* Player = Cast<ASpherePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (Player)
	{
		float Energy = Player->ChargeEnergy(ChargeValue);

		UE_LOG(LogTemp, Warning, TEXT("Energy: %f"), Energy);
	}
}