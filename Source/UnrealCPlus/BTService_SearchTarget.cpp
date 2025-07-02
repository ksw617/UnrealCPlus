// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchTarget.h"
#include "EnemyAIController.h"
#include "MyCharacter.h"
#include "Engine/OverlapResult.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_SearchTarget::UBTService_SearchTarget()
{
	NodeName = TEXT("Search Target");
	Interval = 1.f;
}

void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (Pawn != nullptr)
	{		 
		FVector Center = Pawn->GetActorLocation();
		float SearchDistance = 500.f;
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams QueryParams(NAME_Name, false, Pawn);

		bool Result = GetWorld()->OverlapMultiByChannel
		(
			OverlapResults,
			Center,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel1,
			FCollisionShape::MakeSphere(SearchDistance),
			QueryParams
		);

		if (Result)
		{
			for (auto& OverlapResult : OverlapResults)
			{
				auto Player = Cast<AMyCharacter>(OverlapResult.GetActor());
				if (Player)
				{
					DrawDebugSphere(GetWorld(), Center, SearchDistance, 10, FColor::Green, false, 0.5f);
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Target"), Player);
					return;
				}

			}

		}	
		

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Target"), nullptr);
		DrawDebugSphere(GetWorld(), Center, SearchDistance, 10, FColor::Red, false, 0.5f);

	}

}
