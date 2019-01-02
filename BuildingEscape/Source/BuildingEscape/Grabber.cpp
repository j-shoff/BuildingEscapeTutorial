// Copyright 2018 Perplexed Entertainment

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/EngineTypes.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}
//Grab any actor in range that has physics body collision channel set.
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed!"));

	///LINE TRACE and see if we reach any actors with physics body collision set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	
	///If we hit something then attach a physics handle
	///Attach Physics Handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
	
	
}

//Release the object that we grabbed.
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key released!"));
	
	//TODO release physics handle
	PhysicsHandle->ReleaseComponent();
}

//Look for assumed attached PhysicsHandleComponent
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		//Physics handle found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Missing UPhysicsHandleComponent Component on %s"), *GetOwner()->GetName());
	}
}

//Setup assumed attached InputComponent
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("The UInputComponent was found successfully."));

		//Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Missing UInputComponent on %s"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	///Get the player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;


	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object that we are holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{

	//Physics handle is attached
		//Move the attached object

	///Get the player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	//Draw a red trace in the world to visual
	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);

	///Setup Query Params
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	///Ray-cast out to reach distance
	FHitResult Hit;


	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);


	//See what we hit
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(ActorHit->GetName()));
	}

	return Hit;
}