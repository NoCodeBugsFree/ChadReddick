// Fill out your copyright notice in the Description page of Project Settings.

#include "ChadReddick.h"
#include "Pistol.h"

APistol::APistol()
{
	WeaponConfig.Name = "Pistol";
	WeaponConfig.Priority = 0;
	WeaponConfig.MaxClip = 7;
	WeaponConfig.MaxAmmo = 28;
	CurrentClip = 7;
	WeaponConfig.TimeBetweenShots = 0.3;
}


