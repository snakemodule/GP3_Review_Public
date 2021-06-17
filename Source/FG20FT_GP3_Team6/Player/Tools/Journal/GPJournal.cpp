#include "GPJournal.h"


#include "Camera/CameraComponent.h"
#include "FG20FT_GP3_Team6/Player/GPPlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "FG20FT_GP3_Team6/Player/Tools/Journal/GPPage.h"
#include "Kismet/KismetMathLibrary.h"

AGPJournal::AGPJournal()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGPJournal::BeginPlay()
{
	Super::BeginPlay();
	GPGameMode = Cast<AGPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GetComponents(UChildActorComponent::StaticClass(), Pages, true);
	PageIndex = 0;
	CurrentPage = Cast<UChildActorComponent>(Pages[PageIndex]);
	
	
	//Cast<AGPPage>(CurrentPage->GetChildActor())->MakePageVisible(true);
}

void AGPJournal::OpenJournal()
{
	Cast<AGPPage>(CurrentPage->GetChildActor())->MakePageVisible(true);
}


void AGPJournal::NextPage()
{
	Cast<AGPPage>(CurrentPage->GetChildActor())->MakePageVisible(false);
	PageIndex = (PageIndex + 1) % Pages.Num();
	CurrentPage = Cast<UChildActorComponent>(Pages[PageIndex]);
	Cast<AGPPage>(CurrentPage->GetChildActor())->MakePageVisible(true);
}

void AGPJournal::PreviousPage()
{
	Cast<AGPPage>(CurrentPage->GetChildActor())->MakePageVisible(false);
	PageIndex = (PageIndex - 1) % Pages.Num();
	if(PageIndex < 0)
		PageIndex = Pages.Num()-1;
	CurrentPage = Cast<UChildActorComponent>(Pages[PageIndex]);
	Cast<AGPPage>(CurrentPage->GetChildActor())->MakePageVisible(true);

}

void AGPJournal::ResetTool()
{
	
}


void AGPJournal::InitToolBindings()
{
	UInputComponent* PlayerInputComponent = UGameplayStatics::GetPlayerController(this, 0)->InputComponent;
	PlayerInputComponent->BindAction("Next Page", IE_Pressed, this, &AGPJournal::NextPage);
	PlayerInputComponent->BindAction("Previous Page", IE_Pressed, this, &AGPJournal::PreviousPage);
	PlayerInputComponent->BindAction("Open Book", IE_Pressed, this, &AGPJournal::OpenJournal);
	Cast<AGPPage>(CurrentPage->GetChildActor())->MakePageVisible(true);
}

void AGPJournal::ResetToolBindings()
{
	UInputComponent* PlayerInputComponent = UGameplayStatics::GetPlayerController(this, 0)->InputComponent;
	PlayerInputComponent->RemoveActionBinding("Next Page", IE_Pressed);
	PlayerInputComponent->RemoveActionBinding("Previous Page", IE_Pressed);
	PlayerInputComponent->RemoveActionBinding("Open Book", IE_Pressed);
	Cast<AGPPage>(CurrentPage->GetChildActor())->MakePageVisible(false);
}

void AGPJournal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGPPlayerPawn* Player = Cast<AGPPlayerPawn>(GetParentActor());
	
	auto const Camera = Player->FirstPersonCameraComponent;
	auto CameraForward = Camera->GetForwardVector();
	auto Upness = FVector::DotProduct(FVector::UpVector, CameraForward);
	
	float t = UKismetMathLibrary::NormalizeToRange(Upness, LowerThreshold, UpperThreshold);
	t = FMath::Clamp(t,0.f,1.f);
		
	InterpolatedPosition = FMath::Lerp(LookingPosition,RestingPosition, t);
	
	if (CurrentState == Equipping)
	{
		InterpolatePosition(DeltaTime, EquipStartPosition, InterpolatedPosition, EquipTime);
		if (AnimTimer >= EquipTime && bReverse == false)
		{
			CurrentState = Idle;
		}
		else if (AnimTimer <= 0 && bReverse == true)
		{
			CurrentState = Idle;
			FinalizeSwap(SwappingTo);
		}
	}
	else 
	{
		SetActorRelativeLocation(InterpolatedPosition);
	}
}

void AGPJournal::StartSwap(AGPToolBase* Tool, void (AGPPlayerPawn::*OnSwapEvent)())
{
	if (CurrentState == Idle)
	{
		AnimTimer = EquipTime;
		bReverse = true;
		CurrentState = State::Equipping;
		Super::StartSwap(Tool, OnSwapEvent);		
	}
}

void AGPJournal::Equip()
{
	if (CurrentState == Idle)
	{
		AnimTimer = 0;
		bReverse = false;
		CurrentState = State::Equipping;
	}
}



