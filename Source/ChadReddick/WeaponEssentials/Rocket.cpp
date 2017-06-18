// Fill out your copyright notice in the Description page of Project Settings.

#include "ChadReddick.h"
#include "Rocket.h"
#include "WeaponEssentials/Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ARocket::ARocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Comp"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetSphereRadius(5.f);

	// Event called when a component hits (or is hit by) something solid. This could happen due to things
	// like Character movement, using Set Location with 'sweep' enabled, or physics simulation.
	// For events when objects overlap (e.g. walking into a trigger) see the 'Overlap' event. 
	// For collisions during physics simulation to generate hit events, 'Simulation Generates 
	// Hit Events' must be enabled for this component.
	//	When receiving a hit from another object's movement, the directions of 'Hit.Normal' and 
	// 'Hit.ImpactNormal' will be adjusted to indicate force from the other object against this object.

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ARocket::OnCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = CollisionSphere;
	ProjectileMovement->InitialSpeed = 3000;
	ProjectileMovement->MaxSpeed = 3000;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComponent);

	InitialLifeSpan = 5;
}

// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARocket::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ARocket::OnCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		/** If collides with enemy - kill it  */
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if(Enemy)
		{
			Enemy->Destroy();
		}

		/** Attempt to move target  */
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100, GetActorLocation());

		/** Destroy self  */
		Destroy();
	}
}

