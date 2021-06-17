#include "GPBTTask_SetBlackboardBool.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

UGPBTTask_SetBlackboardBool::UGPBTTask_SetBlackboardBool()
{
	NodeName = TEXT("Set Blackboard Bool");
	bNotifyTick = false;
	
	TargetBlackboardEntry.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UGPBTTask_SetBlackboardBool, TargetBlackboardEntry));
	SourceBlackboardEntry.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UGPBTTask_SetBlackboardBool, SourceBlackboardEntry));
}

EBTNodeResult::Type UGPBTTask_SetBlackboardBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//TODO do thing
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (BlackboardComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	bool bValueToSet = bSourceValue;
	if (SourceEntryType == EGPBTEntryType::BBEntry)
	{
		bValueToSet = BlackboardComp->GetValue<UBlackboardKeyType_Bool>(SourceBlackboardEntry.GetSelectedKeyID());
	}
	
	BlackboardComp->SetValue<UBlackboardKeyType_Bool>(TargetBlackboardEntry.GetSelectedKeyID(), bValueToSet);
	return EBTNodeResult::Succeeded;
}

void UGPBTTask_SetBlackboardBool::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		TargetBlackboardEntry.ResolveSelectedKey(*BBAsset);
		if (SourceEntryType == EGPBTEntryType::BBEntry)
		{
			SourceBlackboardEntry.ResolveSelectedKey(*BBAsset);
		}
	}
}
