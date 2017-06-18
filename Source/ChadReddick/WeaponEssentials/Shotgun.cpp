// Fill out your copyright notice in the Description page of Project Settings.

#include "ChadReddick.h"
#include "Shotgun.h"

AShotgun::AShotgun()
{
	ProjectileType = EProjectileType::ESpread;

	WeaponConfig.Name = "Shotgun";
	WeaponConfig.Priority = 1;
	WeaponConfig.MaxClip = 6;
	WeaponConfig.MaxAmmo = 24;
	WeaponConfig.TimeBetweenShots = 0.5;
	WeaponConfig.WeaponSpread = 10;
	WeaponConfig.bUsePellets = true;
	WeaponConfig.NumberOfPellets = 11;
	CurrentAmmo = 0;
	CurrentClip = 6;
}


