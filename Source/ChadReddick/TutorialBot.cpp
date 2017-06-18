// Fill out your copyright notice in the Description page of Project Settings.

#include "ChadReddick.h"
#include "TutorialBot.h"
#include "BotAIController.h"


// Sets default values
ATutorialBot::ATutorialBot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// AIControllerClass - Default class to use when pawn is controlled by AI.
	AIControllerClass = ABotAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ATutorialBot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATutorialBot::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ATutorialBot::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

