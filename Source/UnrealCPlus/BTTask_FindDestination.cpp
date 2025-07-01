// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindDestination.h"
#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindDestination::UBTTask_FindDestination()
{
	NodeName = TEXT("Find Patrol Destination");
}

EBTNodeResult::Type UBTTask_FindDestination::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (Pawn != nullptr)
	{
		auto NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		if (NavSystem != nullptr)
		{
			FNavLocation RandomLocation;
			if (NavSystem->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), 500.f, RandomLocation))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("Destination"), RandomLocation);
				return EBTNodeResult::Succeeded;
			}

		}

	}

	return EBTNodeResult::Failed;
	
}
