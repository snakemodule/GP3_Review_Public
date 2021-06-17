#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "GPBTTask_SetBlackboardBool.generated.h"

UENUM(BlueprintType)
enum class EGPBTEntryType : uint8
{
	Value,
	BBEntry
};

UCLASS()
class UGPBTTask_SetBlackboardBool : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UGPBTTask_SetBlackboardBool();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetBlackboardEntry;

	UPROPERTY(EditAnywhere)
	EGPBTEntryType SourceEntryType = EGPBTEntryType::Value;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "SourceEntryType == EGPBTEntryType::Value", EditConditionHides))
	bool bSourceValue = true;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "SourceEntryType == EGPBTEntryType::BBEntry", EditConditionHides))
	FBlackboardKeySelector SourceBlackboardEntry;
};