#include "GPDissolveControl.h"

UGPDissolveControl::UGPDissolveControl()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UGPDissolveControl::BeginPlay()
{
	Super::BeginPlay();
	MaterialDynamic = UMaterialInstanceDynamic::Create(GetOwner()->FindComponentByClass<UStaticMeshComponent>()->GetMaterial(0), this);
	SetDissolve(OldValue);
}

void UGPDissolveControl::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TAmount = FMath::Lerp(0.f, 1.f, t);
	SetDissolve(TAmount);
	t += 0.001f;
}


void UGPDissolveControl::SetDissolve(float Amount)
{
	OldValue = Amount;
	float OldRange = OldMax - OldMin;
	float NewRange = NewMax - NewMin;
	NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin;
	MaterialDynamic->SetScalarParameterValue(FName(TEXT("Dissolve")), NewValue);
	GetOwner()->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, MaterialDynamic);
}

void UGPDissolveControl::Interact(UGPInteractionComponent* InteractionComponent)
{
	
}
