// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "UBT_Service_SearchTarget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSTUDYC_API UUBT_Service_SearchTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UUBT_Service_SearchTarget();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
