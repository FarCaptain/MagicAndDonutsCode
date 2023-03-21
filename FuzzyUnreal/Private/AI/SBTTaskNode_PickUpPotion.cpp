// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTaskNode_PickUpPotion.h"

#include "AIController.h"
#include "SGameplayInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTaskNode_PickUpPotion::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if(ensure(AIController))
	{
		ACharacter* AICharacter = Cast<ACharacter> (AIController->GetPawn());
		if(AICharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		AActor* TargetPotion = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetPotion"));

		if(TargetPotion == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if(!TargetPotion->Implements<USGameplayInterface>())
		{
			return EBTNodeResult::Failed;
		}
		
		ISGameplayInterface::Execute_Interact(TargetPotion, AICharacter);
		return  EBTNodeResult::Succeeded;
	}

	return  EBTNodeResult::Failed;
}
