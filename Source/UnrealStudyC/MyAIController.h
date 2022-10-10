// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSTUDYC_API AMyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMyAIController();
	virtual void OnPossess(APawn* InPawn)override;
	virtual void OnUnPossess()override;


private:
	FTimerHandle TimerHandle;
	void RandomeMove();
	UPROPERTY()
	class UBehaviorTree* BehaviorTree;
	UPROPERTY()
	class UBlackboardData* BlackboardData;
};
