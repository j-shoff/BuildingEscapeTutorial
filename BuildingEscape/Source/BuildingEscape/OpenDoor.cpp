// Copyright 2018 Perplexed Entertainment

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	//Set the door open rotation
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{

	//Set the door closed rotation
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Check if has ActorThatOpens is overlapping PressurePlate
	if ((ActorThatOpens != nullptr) && (PressurePlate != nullptr))
	{
		if (PressurePlate->IsOverlappingActor(ActorThatOpens))
		{
			OpenDoor();
			LastDoorOpenTime = GetWorld()->GetTimeSeconds();
		}
	}

	//Check if it is time to close the door.
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

