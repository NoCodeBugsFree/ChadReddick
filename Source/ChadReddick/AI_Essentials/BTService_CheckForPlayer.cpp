// Fill out your copyright notice in the Description page of Project Settings.

#include "ChadReddick.h"
#include "BTService_CheckForPlayer.h"
#include "AI_Essentials/EnemyCharacter.h"
#include "AI_Essentials/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTService_CheckForEnemy::UBTService_CheckForEnemy()
{
	bCreateNodeInstance = true;
}

void UBTService_CheckForEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	/** Get the AI_Controller  */
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (EnemyAIController)
	{
		/** Grab a target   */
		APawn* CharacterPawn = Cast<APawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (CharacterPawn)
		{
			/** Store this target in BB  */
			OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", CharacterPawn);
		}
	}
}
