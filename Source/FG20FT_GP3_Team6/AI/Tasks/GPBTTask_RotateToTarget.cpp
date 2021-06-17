#include "GPBTTask_RotateToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UGPBTTask_RotateToTarget::UGPBTTask_RotateToTarget() : Super()
{
	NodeName = TEXT("Smooth Rotate To Target");
	bNotifyTick = true;

	TargetLocationEntry.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UGPBTTask_RotateToTarget, TargetLocationEntry));
	TargetLocationEntry.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UGPBTTask_RotateToTarget, TargetLocationEntry), AActor::StaticClass());
}

EBTNodeResult::Type UGPBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UGPBTTask_RotateToTarget::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UGPBTTask_RotateToTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	// do nothing?
}

void UGPBTTask_RotateToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController)
		return;

	APawn* PawnOwner = AIController->GetPawn();

	if (!PawnOwner)
		return;

	bool bSuccess = false;
	const FVector TargetLocation = GetTargetLocation(OwnerComp, bSuccess);

	if (!bSuccess)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const FVector DirectionToTarget = (TargetLocation - PawnOwner->GetActorLocation()).GetSafeNormal();
	FQuat Rot = FQuat::Slerp(PawnOwner->GetActorQuat(), DirectionToTarget.ToOrientationQuat(), DeltaSeconds * RotationSpeedValue);

	FRotator NewRot = Rot.Rotator();
	NewRot.Pitch = 0.0f;
	NewRot.Roll = 0.0f;

	PawnOwner->SetActorRotation(NewRot);

	if (FMath::IsNearlyEqual(DirectionToTarget.Rotation().Yaw, NewRot.Yaw, AcceptableTolerance))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UGPBTTask_RotateToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		TargetLocationEntry.ResolveSelectedKey(*BBAsset);
	}
}

FVector UGPBTTask_RotateToTarget::GetTargetLocation(const UBehaviorTreeComponent& OwnerComp, bool& bSuccess) const
{
	bSuccess = true;

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (BlackboardComp != nullptr)
	{
		if (TargetLocationEntry.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
		{
			UObject* TargetObject = BlackboardComp->GetValue<UBlackboardKeyType_Object>(TargetLocationEntry.GetSelectedKeyID());
			if (TargetObject != nullptr)
			{
				return CastChecked<AActor>(TargetObject)->GetActorLocation();
			}
		}
		else if (TargetLocationEntry.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			return BlackboardComp->GetValue<UBlackboardKeyType_Vector>(TargetLocationEntry.GetSelectedKeyID());
		}
	}

	bSuccess = false;

	return FVector::ZeroVector;
}


