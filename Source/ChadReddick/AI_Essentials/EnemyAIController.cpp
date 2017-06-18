// Fill out your copyright notice in the Description page of Project Settings.

#include "ChadReddick.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "EnemyCharacter.h"

AEnemyAIController::AEnemyAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Comp"));

	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Comp"));
}

void AEnemyAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(InPawn);

	/** if I am the EnemyCharacter and I have a valid Behavior Tree  */
	if(EnemyCharacter && EnemyCharacter->BotBehavior)
	{
		/** Initialize my Blackboard Component from this Behavior Tree */
		BlackboardComp->InitializeBlackboard(*EnemyCharacter->BotBehavior->BlackboardAsset);

		/** Set up my key  */
		EnemyKeyID = BlackboardComp->GetKeyID("Target");
		
		/** Start the Tree  */
		BehaviorTreeComp->StartTree(*EnemyCharacter->BotBehavior);
	}
}
