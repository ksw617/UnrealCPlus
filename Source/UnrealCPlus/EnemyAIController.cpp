// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemyAIController::AEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Game/AI/BT_Enemy.BT_Enemy"));
	if (BT.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("BehaviorTree"));
		BehaviorTree = BT.Object;
	
	}
	
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("/Game/AI/BB_Enemy.BB_Enemy"));
	if (BT.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("BlackboardData"));
		BlackboardData = BD.Object;
	
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (RunBehaviorTree(BehaviorTree))
	{
		UE_LOG(LogTemp, Log, TEXT("RunBehaviorTree"));
	}
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
