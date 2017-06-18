// Fill out your copyright notice in the Description page of Project Settings.

#include "ChadReddick.h"
#include "BTTask_MoveToPlayer.h"
#include "AI_Essentials/EnemyAIController.h"
#include "AI_Essentials/EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_MoveToPlayerCharacter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/** Get the AI_Controller  */
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	/** Grab a player pawn from BB   */
	APawn* PlayerPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	if (PlayerPawn)
	{
		EnemyAIController->MoveToActor(PlayerPawn, 5.f);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
