// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "ChadReddickCharacter.generated.h"

UCLASS(config=Game)
class AChadReddickCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:

	AChadReddickCharacter();

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	// -----------------------------------------------------------------------------------
	//			FPS / TPS Mode
	// -----------------------------------------------------------------------------------
	
	/** Shows whether character is in FPS or TPS state  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	bool bIsInFPS = true;
	
	/** Calls after zoom in/out  */
	void HandleFpsTpsMode();
	
	/** Calls for smooth change TPS state to FPS  */
	void ZoomIn();

	/**  Calls for smooth change FPS state to TPS  */
	void ZoomOut();

	// -----------------------------------------------------------------------------------
	//			Green Ability
	// -----------------------------------------------------------------------------------

	/** Shows whether character IsCharging now or not   */				
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	bool bIsCharging = false;
	
	/** Green ability class to spawn  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<class AGreenAbility> GreenAbilityClass;

	/** Amount of power and scale of green ability   */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float ChargeAmount = 0.5f;

	/** FX which started when character starts charging */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	class UParticleSystemComponent* GreenCharge;

	/** Calls to fire Green Ability  */
	void FireGreenAbility();

	/** Timer which increase the ChargeAmount property  */
	FTimerHandle ChargeTimer;

	/** Calls to start charging Green Ability  */
	void StartCharging();

	/** Calls to start charging Green Ability  */
	void StopCharging();

	/** Calls when ChargeTimer ticked  */
	void Charge();

	// -----------------------------------------------------------------------------------

	/** if we have a weapon - try to fire  */
	UFUNCTION()
	void FireWeapon();

	/** Default character's weapon   */
	UPROPERTY(EditDefaultsOnly, Category = "DefaultInventory")
	TSubclassOf<class AWeapon> WeaponSpawn;

	/** Current weapon reference  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	class AWeapon* CurrentWeapon;

	/**  Handles picking up of pickups  */
	UFUNCTION()
	void OnCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	
	/** Character's inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TArray<class AWeapon*> Inventory;

	/** Calls when we need to pickup a weapon  */
	void ProcessWeaponPickup(AWeapon* Weapon);

	/** Calls to switch to next weapon  */
	void NextWeapon();

	/** Calls to switch to previous weapon  */
	void PreviousWeapon();

	/** Calls to equip the weapon  */
	void EquipWeapon(class AWeapon* Weapon);

	/** Calls on begin play to give default weapon  */
	void GiveDefaultWeapon();

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

