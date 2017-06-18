// Fill out your copyright notice in the Description page of Project Settings.

#include "ChadReddick.h"
#include "GreenAbility.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AGreenAbility::AGreenAbility()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	CollisionComponent->InitSphereRadius(15);
	RootComponent = CollisionComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3;

	GreenScale = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Green Scale"));
	GreenScale->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

// Called when the game starts or when spawned
void AGreenAbility::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGreenAbility::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

