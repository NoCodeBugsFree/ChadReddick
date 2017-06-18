// Fill out your copyright notice in the Description page of Project Settings.

#include "ChadReddick.h"
#include "RocketLauncher.h"
#include "Rocket.h"

ARocketLauncher::ARocketLauncher()
{
	ProjectileType = EProjectileType::EProjectile;

	WeaponConfig.Priority = 2;
	WeaponConfig.Name = "Rocket Launcher";
	WeaponConfig.MaxClip = 2;
	WeaponConfig.MaxAmmo = 4;
	WeaponConfig.TimeBetweenShots = 3;
	CurrentAmmo = 1;
}

void ARocketLauncher::FireProjectile()
{
	Super::FireProjectile();

	/** Simple spawn Rocket  */
	if (ProjectileClass)
	{
		FVector MFLoc = WeaponMesh->GetSocketLocation("MF");
		FRotator MFRot = WeaponMesh->GetSocketRotation("MF");
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		ARocket* Rocket = GetWorld()->SpawnActor<ARocket>(ProjectileClass, MFLoc, MFRot, SpawnParams);
	}
}
