// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BotAIController.generated.h"

/**
 * 
 */
UCLASS()
class CHADREDDICK_API ABotAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ABotAIController();

	virtual void Tick(float DeltaTime) override;

	/** Find a nearest Player character and set it reference and location in BB  */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
	void SearchForEnemy();

protected:

	// Blackboard keys  

	/** Enemy - Object in BB  */
	uint8 EnemyKeyId;

	/** Destination - Vector in BB  */
	uint8 EnemyLocationId;

private:

	UPROPERTY(Transient)
	class UBlackboardComponent* BlackboardComp;
	
	UPROPERTY(Transient)
	class UBehaviorTreeComponent* BehaviorTreeComp;

	/** Calls when AIController is attaching to Pawn  */
	virtual void Possess(APawn* InPawn) override;
	
	/** Sets the received data to BB  */
	void SetEnemy(APawn* InPawn);

};
