// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "TutorialBot.generated.h"

UCLASS()
class CHADREDDICK_API ATutorialBot : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATutorialBot();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Behavior Tree for this Bot */
	UPROPERTY(EditAnywhere, Category = "AAA")
	UBehaviorTree* BehaviorTree;
	
};
