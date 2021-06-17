#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "GPBTTask_RotateToTarget.generated.h"

UENUM(BlueprintType)
enum class EGPSmoothFaceDirectionSpeedType : uint8
{
	Value,
	BBEntry
};

UCLASS()
class UGPBTTask_RotateToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UGPBTTask_RotateToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	FVector GetTargetLocation(const UBehaviorTreeComponent& OwnerComp, bool& bSuccess) const;

	UPROPERTY(EditAnywhere, Category = Entry)
	FBlackboardKeySelector TargetLocationEntry;
	
	UPROPERTY(EditAnywhere, Category = Entry, meta = (DisplayName = "Rotation Speed")) // ClampMin?
	float RotationSpeedValue = 2.0f;

	UPROPERTY(EditAnywhere, Category = Entry, meta = (ClampMin = "0.0"))
	float AcceptableTolerance = 1.0f;
};

