// Fill out your copyright notice in the Description page of Project Settings.


#include "UBT_Service_SearchTarget.h"
#include "MyAIController.h"
#include "MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UUBT_Service_SearchTarget::UUBT_Service_SearchTarget()
{
	NodeName = TEXT("SearchTarget");
	Interval = 1.f;
}

void UUBT_Service_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (CurrentPawn == nullptr)
	{
		return;
	}

	UWorld* World = CurrentPawn->GetWorld();
	FVector Center = CurrentPawn->GetActorLocation();
	float SearchRadius = 300.f;
	float Sensing = 500.f;

	if (World == nullptr)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams  QueryParams(NAME_None, false, CurrentPawn);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Sensing),
		QueryParams
	);

	if (bResult)
	{
		for (auto& Result : OverlapResults)
		{
			AMyCharacter* MyCharacter = Cast<AMyCharacter>(Result.GetActor());
			if (MyCharacter && MyCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), MyCharacter);

				DrawDebugSphere(World, Center, Sensing, 16, FColor::Green, false, 0.2f);
				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
	}
	DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);



}