// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ChadReddick.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "ChadReddickCharacter.h"
#include "GreenAbility.h"
#include "WeaponEssentials/Weapon.h"
#include "Engine.h"
#include "Engine/Blueprint.h"
#include "WeaponEssentials/Pistol.h"
#include "WeaponEssentials/Shotgun.h"
#include "WeaponEssentials/RocketLauncher.h"

//////////////////////////////////////////////////////////////////////////
// AChadReddickCharacter

AChadReddickCharacter::AChadReddickCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	/** Handles picking up of pickups   */
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AChadReddickCharacter::OnCollision);


	// void TArray::SetNum(int32 NewNum, bool bAllowShrinking) - 
	// Resizes array to given number of elements.
	// NewNum - New size of the array.
	// bAllowShrinking - Tell if this function can shrink the memory in-use if suitable
	
	Inventory.SetNum(3, false);

	// -----------------------------------------------------------------------------------

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Particles
	GreenCharge = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Green Charge"));
	GreenCharge->SetupAttachment(RootComponent);
	GreenCharge->bAutoActivate = false;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AChadReddickCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AChadReddickCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AChadReddickCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AChadReddickCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AChadReddickCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AChadReddickCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AChadReddickCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AChadReddickCharacter::OnResetVR);

	// Zoom
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &AChadReddickCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &AChadReddickCharacter::ZoomOut);

	// Green
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AChadReddickCharacter::FireWeapon);
	PlayerInputComponent->BindAction("FireGreen", IE_Pressed, this, &AChadReddickCharacter::FireGreenAbility);
	PlayerInputComponent->BindAction("Charge", IE_Pressed, this, &AChadReddickCharacter::StartCharging);
	PlayerInputComponent->BindAction("Charge", IE_Released, this, &AChadReddickCharacter::StopCharging);

	// Equip

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &AChadReddickCharacter::NextWeapon);
	PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, this, &AChadReddickCharacter::PreviousWeapon);
}

void AChadReddickCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GiveDefaultWeapon();
}

void AChadReddickCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChadReddickCharacter::HandleFpsTpsMode()
{
	if (bIsInFPS)
	{
		CameraBoom->TargetArmLength = 0.0f;
		CameraBoom->SetRelativeLocation(FVector(0, -5, 0));
		CameraBoom->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "sHead");

		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = true;
		bUseControllerRotationRoll = true;
	}
	else
	{
		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
		bUseControllerRotationRoll = false;

		CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void AChadReddickCharacter::ZoomIn()
{
	if (CameraBoom->TargetArmLength > 75.f)
	{
		bIsInFPS = false;
		CameraBoom->TargetArmLength -= 75.f;
		CameraBoom->SetRelativeLocation(FVector(0.0f, 50.f, 50.f));
	}
	else
	{
		bIsInFPS = true;
	}
	HandleFpsTpsMode();
}

void AChadReddickCharacter::ZoomOut()
{
	if (CameraBoom->TargetArmLength < 225.f)
	{
		bIsInFPS = false;
		CameraBoom->TargetArmLength += 75.f;
		CameraBoom->SetRelativeLocation(FVector(0.0f, 50.f, 50.f));
	}
	else
	{
		CameraBoom->TargetArmLength = 300;
		CameraBoom->SetRelativeLocation(FVector::ZeroVector);
	}
	HandleFpsTpsMode();
}

void AChadReddickCharacter::FireGreenAbility()
{
	bIsCharging = false;

	FVector SocketLocation = GetMesh()->GetSocketLocation("ProjectileSocket");
	FRotator SocketRotation = GetMesh()->GetSocketRotation("ProjectileSocket");
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	UWorld* const World = GetWorld();
	if (World)
	{
		AGreenAbility* SpawnGreen = World->SpawnActor<AGreenAbility>(GreenAbilityClass, SocketLocation, SocketRotation, SpawnParams);
		if (SpawnGreen)
		{
			SpawnGreen->SetActorScale3D(FVector(ChargeAmount, ChargeAmount, ChargeAmount));
		}
	}
	ChargeAmount = 0.5f;
}

void AChadReddickCharacter::StartCharging()
{
	GetWorldTimerManager().SetTimer(ChargeTimer, this, &AChadReddickCharacter::Charge, 0.1f, true);
	bIsCharging = true;
	GreenCharge->Activate();
}

void AChadReddickCharacter::StopCharging()
{
	GetWorldTimerManager().ClearTimer(ChargeTimer);
	bIsCharging = false;
	GreenCharge->Deactivate();
	ChargeAmount = 0.5;
}

void AChadReddickCharacter::Charge()
{
	if (bIsCharging)
	{
		ChargeAmount += 0.03;
		FMath::Clamp(ChargeAmount, 0.f, 3.f);
	}
}

void AChadReddickCharacter::FireWeapon()
{
	if (!CurrentWeapon) 
	{
		UE_LOG(LogTemp, Warning, TEXT("No Weapon in hands ..."));
		return;
	}
	CurrentWeapon->Fire();
}

void AChadReddickCharacter::OnCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWeapon* Weapon = Cast<AWeapon>(OtherActor);
	if (Weapon)
	{
		ProcessWeaponPickup(Weapon);
	}
}

void AChadReddickCharacter::ProcessWeaponPickup(AWeapon * Weapon)
{
	if (Weapon)
	{
		// check the slot by priority
		if (Inventory[Weapon->WeaponConfig.Priority] == nullptr) // if nothing here
		{
			AWeapon* Spawner = GetWorld()->SpawnActor<AWeapon>(Weapon->GetClass());
			if (Spawner)
			{
				// if valid put this to inventory
				Inventory[Spawner->WeaponConfig.Priority] = Spawner;
				UE_LOG(LogTemp, Warning, TEXT("You just pick up a %s"), *Inventory[Spawner->WeaponConfig.Priority]->WeaponConfig.Name);
			}
			Weapon->Destroy(); 
		}
		else
		{

			if (Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo >= 0 && 
				Weapon->CurrentAmmo <= 
				Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo - 
				Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo)
			{
				Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo += Weapon->CurrentAmmo;
				UE_LOG(LogTemp, Warning, TEXT("added %i"), Weapon->CurrentAmmo); // TODO check here
				Weapon->Destroy();
			}
			else
			{
				if (Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo > Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo)
				{
					Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo = Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo;
				}
			}
		}
	}
}

void AChadReddickCharacter::NextWeapon()
{
	if (Inventory[CurrentWeapon->WeaponConfig.Priority]->WeaponConfig.Priority != 2) // TODO remove !=2 HERE
	{
		if (Inventory[CurrentWeapon->WeaponConfig.Priority + 1] == nullptr)
		{
			for (int32 i = CurrentWeapon->WeaponConfig.Priority + 1; i < Inventory.Num(); i++)
			{
				if (Inventory[i] && Inventory[i]->IsA(AWeapon::StaticClass()))
				{
					EquipWeapon(Inventory[i]);
				}
			}
		} 
		else
		{
			EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority + 1]);
		}
	}
	else
	{
		EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority]);
	}
}

void AChadReddickCharacter::PreviousWeapon()
{
	if (Inventory[CurrentWeapon->WeaponConfig.Priority]->WeaponConfig.Priority != 0)
	{
		if (Inventory[CurrentWeapon->WeaponConfig.Priority - 1] == nullptr)
		{
			for (int32 i = CurrentWeapon->WeaponConfig.Priority - 1; i >= 0; i--)
			{
				if (Inventory[i] && Inventory[i]->IsA(AWeapon::StaticClass()))
				{
					EquipWeapon(Inventory[i]);
				}
			}
		}
		else
		{
			EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority - 1]);
		}
	}
	else
	{
		EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority]);
	}
}

void AChadReddickCharacter::EquipWeapon(AWeapon * Weapon)
{	
	if(CurrentWeapon)
	{
		CurrentWeapon = Inventory[CurrentWeapon->WeaponConfig.Priority];
		CurrentWeapon->OnUnEquip();
		CurrentWeapon = Weapon;
		Weapon->SetOwningPawn(this);
		Weapon->OnEquip();
	}
	else
	{
		CurrentWeapon = Weapon;
		CurrentWeapon = Inventory[CurrentWeapon->WeaponConfig.Priority];
		CurrentWeapon->SetOwningPawn(this);
		Weapon->OnEquip();
	}
}

void AChadReddickCharacter::GiveDefaultWeapon()
{
	AWeapon * Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn);
	if (Spawner)
	{
		Inventory[Spawner->WeaponConfig.Priority] = Spawner;
		CurrentWeapon = Inventory[Spawner->WeaponConfig.Priority];
		CurrentWeapon->SetOwningPawn(this);
		CurrentWeapon->OnEquip();
	}
}


// -----------------------------------------------------------------------------------

void AChadReddickCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AChadReddickCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AChadReddickCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AChadReddickCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AChadReddickCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AChadReddickCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AChadReddickCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
