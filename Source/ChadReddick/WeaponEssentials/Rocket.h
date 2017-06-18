// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Rocket.generated.h"

UCLASS()
class CHADREDDICK_API ARocket : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	ARocket();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Rocket Projectile Movement Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	/** Rocket Mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

private:
	
	/** Collision Sphere for this Actor */
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionSphere;
	 
	/** Calls when Rocket starts overlap another actor  */
	UFUNCTION()
	void OnCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
