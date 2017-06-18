// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GreenAbility.generated.h"

UCLASS()
class CHADREDDICK_API AGreenAbility : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AGreenAbility();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Growing particles to visualize power of ability  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particle")
	UParticleSystemComponent* GreenScale;

protected:

	/**  Projectile Movement Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particle")
	class UProjectileMovementComponent* ProjectileMovement;

	/** Simple sphere collision  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particle")
	USphereComponent* CollisionComponent;
	
};
