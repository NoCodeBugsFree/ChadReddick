// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponEssentials/Weapon.h"
#include "RocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class CHADREDDICK_API ARocketLauncher : public AWeapon
{
	GENERATED_BODY()
	
public:

	ARocketLauncher();
	
	virtual void FireProjectile() override;
	
};
