// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckForPlayer.generated.h"

/**
 * 
 */
UCLASS()
class CHADREDDICK_API UBTService_CheckForEnemy : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTService_CheckForEnemy();

	/** Tick every defined interval - default 0.4-0.6 sec  */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:



private:
	
	
};
