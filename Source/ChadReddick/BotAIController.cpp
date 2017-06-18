// Fill out your copyright notice in the Description page of Project Settings.

#include "ChadReddick.h"
#include "BotAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "TutorialBot.h"
#include "ChadReddickCharacter.h"

ABotAIController::ABotAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Comp"));

	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Comp"));
}

void ABotAIController::Possess(APawn* InPawn)
{
	// AAIController::Possess
	// Handles attaching this controller to the specified pawn
	Super::Possess(InPawn);

	ATutorialBot* Bot = Cast<ATutorialBot>(InPawn);

	if (Bot && Bot->BehaviorTree)
	{
		// Setup component for using given blackboard asset
		BlackboardComp->InitializeBlackboard(*Bot->BehaviorTree->BlackboardAsset);

		// Key ID from name
		EnemyKeyId = BlackboardComp->GetKeyID("Enemy"); // Enemy - Object in B
		EnemyLocationId = BlackboardComp->GetKeyID("Destination"); // Vector in BB 

		// Starts execution from root
		BehaviorTreeComp->StartTree(*Bot->BehaviorTree);
	}
}

void ABotAIController::SetEnemy(APawn* InPawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject("Enemy", InPawn);
		BlackboardComp->SetValueAsVector("Destination", InPawn->GetActorLocation());
	}
}

void ABotAIController::SearchForEnemy()
{
	// AController::GetPawn - Getter for Pawn
	APawn* MyBot = GetPawn();
	if (!MyBot) { return; }

	const FVector MyBotLocation = MyBot->GetActorLocation();

	float BestDistSq = MAX_FLT;

	AChadReddickCharacter* BestPawn = nullptr;

	// FConstPawnIterator GetPawnIterator()
	// Returns an iterator for the pawn list.
	for (auto It = GetWorld()->GetPawnIterator(); It; It++)
	{
		AChadReddickCharacter* TestPawn = Cast<AChadReddickCharacter>(*It);
		if (TestPawn)
		{
			// The distance between two points.
			const float DistSq = FVector::Dist(TestPawn->GetActorLocation(), MyBotLocation);
			if (DistSq < BestDistSq)
			{
				BestDistSq = DistSq;
				BestPawn = TestPawn;
			}
		}
	}

	if (BestPawn)
	{
		SetEnemy(BestPawn);
	}
}

void ABotAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
