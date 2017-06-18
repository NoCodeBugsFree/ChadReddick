// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WeaponEssentials/Rocket.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Weapon.generated.h"

#define TRACE_WEAPON ECC_GameTraceChannel1

/** Define types of projectiles  */
UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	EBullet UMETA(DisplayName = "Bullet"),
	ESpread UMETA(DisplayName = "Spread"),
	EProjectile UMETA(DisplayName = "Projectile")
};

/**  Define weapon struct   */
USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()
	
	/** Max amount of clips available to carry for this type of weapon  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 MaxClip;

	/** Max available amount of ammo for this type of weapon  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MaxAmmo = 10;
	
	/** TODO doesn't work  */
	/** Reloading time for this type of weapon  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float TimeBetweenShots = 0.1f;

	/** Defines how many ammo costs one shot  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 ShotCost = 1;

	/** Maximum range for this type of weapon  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float WeaponRange = 1000.f;

	/** Cone spread for this type of weapon   */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (EditCondition = "bUsePellets"))
	float WeaponSpread = 0.0f;

	/** Number of pellets per shot  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (EditCondition = "bUsePellets"))
	int32 NumberOfPellets = 10;

	/** Shows whether or not this weapon use pellets multi shots like shotgun  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	bool bUsePellets = false;

	/** Display name   for this type of weapon  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FString Name = "Enter Weapon Name Here ... ";

	/** simple picture - UI representation  for this type of weapon   */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	UTexture2D* SplashArt;

	/** Priority defines inventory order  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 Priority;

};

// FD
class AChadReddickCharacter;

UCLASS()
class CHADREDDICK_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Attempt to fire a weapon - calls whenever player press fire button */
	UFUNCTION()
	void Fire();

	/** Base empty function to fire projectiles */
	UFUNCTION()
	virtual void FireProjectile();
	
	/**  Sets new owning pawn for this actor */
	void SetOwningPawn(AChadReddickCharacter * NewOwner);

	/**  Attaches this weapon to character's gun socket */
	void AttachToPlayer();
	
	/** Detaches current weapon from character's gun socket  */
	void DetachFromPlayer();

	/** Disables weapon collision and attaches it to character's gun socket  */
	void OnEquip();
	
	/** Detaches current weapon from character's gun socket  */
	void OnUnEquip();

	/** Calls when clip is over and try to fill the clip if possible */
	void ReloadAmmo();

	/** Calls whenever weapon shoot  */
	UAudioComponent* PlayWeaponSound(USoundCue* Sound);

	/** Struct which defines all weapon essential attributes  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FWeaponData WeaponConfig;

	/** Type of projectile for this weapon  */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ARocket> ProjectileClass;

	/** Fire Sound asset */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	USoundCue* FireSound;

	/** Amount of total ammo for this weapon   */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 CurrentAmmo;

	/** Amount of ammo for current clip */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 CurrentClip;

	/** Defines the type of weapon by it's projectiles type  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	EProjectileType ProjectileType;

	/**  Simple box collision for pickup interaction*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* BoxCollision;

	/** Represent weapon 3D weapon  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	USkeletalMeshComponent* WeaponMesh;

protected:

	/** Character's reference to attach    */
	UPROPERTY()
	AChadReddickCharacter* MyPawn;

	/** Performs a ray cast and returns HitResult if success   */
	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

	/** Calls when shoot another actor to apply damage to this guy   */
	void ProcessInstantHit(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir, int32 RandomSeed, float ReticleSpread);

private:

	/** The sum function for shooting logic 
		- calls if shot if available
		- calculates spread
		- perform weapon trace
		- process instant hit
	*/
	UFUNCTION()
	void InstantFire();

};
