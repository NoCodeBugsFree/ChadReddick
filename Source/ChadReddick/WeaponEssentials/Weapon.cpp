// Fill out your copyright notice in the Description page of Project Settings.

#include "ChadReddick.h"
#include "Weapon.h"
#include "Engine.h"
#include "Enemy.h"
#include "ChadReddickCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Comp"));
	BoxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AWeapon::Fire()
{
	// Bullet - pistol, rifle
	if (ProjectileType == EProjectileType::EBullet)
	{
		if (CurrentClip > 0)
		{
			InstantFire();
			PlayWeaponSound(FireSound);
			CurrentClip -= WeaponConfig.ShotCost;
		}
		else
		{
			ReloadAmmo();
		}
	}

	// Spread - shotgun
	if (ProjectileType == EProjectileType::ESpread)
	{
		if (CurrentClip > 0)
		{
			for (int32 i = 0; i < WeaponConfig.NumberOfPellets; i++)
			{
				InstantFire();
			}
			PlayWeaponSound(FireSound);
			CurrentClip -= WeaponConfig.ShotCost;
		}
		else
		{
			ReloadAmmo();
		}
	}

	// Projectile - rocket launcher
	if (ProjectileType == EProjectileType::EProjectile)
	{
		if (CurrentClip > 0)
		{
			FireProjectile();
			CurrentClip -= WeaponConfig.ShotCost;
			PlayWeaponSound(FireSound);
		}
		else
		{
			ReloadAmmo();
		}
	}
}

void AWeapon::FireProjectile()
{
	UE_LOG(LogTemp, Warning, TEXT("AWeapon::FireProjectile() called ..."));
}

void AWeapon::SetOwningPawn(AChadReddickCharacter * NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

void AWeapon::AttachToPlayer()
{
	if(MyPawn)
	{
		// detach old one and attach new one
		DetachFromPlayer();

		WeaponMesh->SetHiddenInGame(false);
		WeaponMesh->AttachToComponent(MyPawn->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale ,"WeaponSocket");
	}
}

void AWeapon::DetachFromPlayer()
{ 
	WeaponMesh->SetHiddenInGame(true);
}

void AWeapon::OnEquip()
{
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToPlayer();
}

void AWeapon::OnUnEquip()
{
	DetachFromPlayer();
}

void AWeapon::ReloadAmmo()
{
	// TODO delay and play animation 

	/**  if we have at least 1 ammo ... */
	if (CurrentAmmo > 0)
	{
		/** check whether we have enough ammo to fill current clip completely  */
		if (CurrentAmmo < WeaponConfig.MaxClip)
		{
			/** we haven't enough ammo to fill  current clip completely - fill only by CurrentAmmo value */
			CurrentClip = CurrentAmmo;
		}
		else /**  we haven enough ammo  */
		{
			// reduce current ammo by WeaponConfig.MaxClip value
			CurrentAmmo -= WeaponConfig.MaxClip;

			// add this ammo to current clip
			CurrentClip += WeaponConfig.MaxClip;
		}
	}
	else // out of ammos
	{
		// TODO play out of ammo sound FX
	}
}

UAudioComponent* AWeapon::PlayWeaponSound(USoundCue* Sound)
{
	UAudioComponent* AudioComponent = nullptr;
	if(Sound && MyPawn)
	{
		AudioComponent = UGameplayStatics::SpawnSoundAttached(Sound, MyPawn->GetRootComponent());
	}

	return AudioComponent;
}

void AWeapon::InstantFire()
{	
	/**  Calculates spread  */
	
	const int32 RandomSeed = FMath::Rand();

	// FRandomStream - Implements a thread-safe SRand based RNG. 
	// Very bad quality in the lower bits.Don't use the modulus (%) operator.
	FRandomStream WeaponRandomStream(RandomSeed);

	float CurrentSpread = WeaponConfig.WeaponSpread;
	float SpreadCone = FMath::DegreesToRadians(WeaponConfig.WeaponSpread * 0.5);

	FVector AimDirection = WeaponMesh->GetSocketRotation("MF").Vector();
	
	FVector StartTrace = WeaponMesh->GetSocketLocation("MF");

	/**  
		 FRandomStream::VRandCone(FVector const& Dir, float HorizontalConeHalfAngleRad, float VerticalConeHalfAngleRad ) -
			Returns a random unit vector, uniformly distributed, within the specified cone.
		 
		 FVector FMath::VRandCone(FVector const & Dir, float HorizontalConeHalfAngleRad, float VerticalConeHalfAngleRad)
		 Dir - The center direction of the cone
		 HorizontalConeHalfAngleRad - Horizontal half-angle of cone, in radians.
		 VerticalConeHalfAngleRad   - Vertical half-angle of cone, in radians.
	*/
	FVector ShootDirection = WeaponRandomStream.VRandCone(AimDirection, SpreadCone, SpreadCone);

	FVector EndTrace = StartTrace + ShootDirection * WeaponConfig.WeaponRange;

	
	/**  perform weapon trace  */
	FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	/** process instant hit  */
	ProcessInstantHit(Impact, StartTrace, ShootDirection, RandomSeed, CurrentSpread);
}

FHitResult AWeapon::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	// Structure that defines parameters passed into collision function
	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);

	// Whether we should perform the trace in the asynchronous scene. Default is false.
	TraceParams.bTraceAsyncScene = true;

	// Only fill in the PhysMaterial field of
	TraceParams.bReturnPhysicalMaterial = true;

	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	if (UWorld* World = GetWorld())
	{
		World->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, TRACE_WEAPON, TraceParams);
	}

	return Hit;
}

void AWeapon::ProcessInstantHit(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir, int32 RandomSeed, float ReticleSpread)
{
	FVector EndTrace = Origin + ShootDir * WeaponConfig.WeaponRange;
	FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;
	DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::MakeRandomColor(), true, 10000, 10.f);


	/** TODO implement correct inflict damage functionality here  */
	AEnemy* Enemy = Cast<AEnemy>(Impact.GetActor());
	if (Enemy)
	{
		Enemy->Destroy();
	}

}


