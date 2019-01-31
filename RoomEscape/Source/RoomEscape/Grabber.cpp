// Copyright Alice Meng 2019

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Public/DrawDebugHelpers.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();


	UE_LOG(LogTemp, Warning, TEXT("grabber reporting for duty!"));

	/// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle) {
		// Physics handle is found
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s is missing Physics Handle component"), *(GetOwner()->GetName()));
	}

	// Look for attached Input Handle
	InputHandle = GetOwner()->FindComponentByClass<UInputComponent>();
	
	if (InputHandle) {
		// Input handle is found
		UE_LOG(LogTemp, Warning, TEXT("%s has an Input Handle"), *(GetOwner()->GetName()));

		/// Bind the input axis for grab and release
		InputHandle->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputHandle->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s is missing Input Handle component"), *(GetOwner()->GetName()));
	}
	
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("%s has GRABBED"), *(GetOwner()->GetName()));
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("%s has RELEASED"), *(GetOwner()->GetName()));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if movable object exists in front of me
	// check to see if i'm clicking
		// if so, have the object lifter (translate it)
	// otherwiseput the object down

	// get player viewpoint with tick
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
				OUT PlayerViewpointLocation,
				OUT PlayerViewpointRotation);

	/*UE_LOG(LogTemp, Warning, TEXT("%s Location, Rotation %s"), *PlayerViewpointLocation.ToString(), *PlayerViewpointRotation.ToString());
*/

	/// draw a red in the world to visualize
	///FVector LineTraceEnd = PlayerViewpointLocation + FVector(0.f, 0.f, 50.f);
	FVector UnitRotation;
	PlayerViewpointRotation.Quaternion().Vector();
	FVector LineTraceEnd = PlayerViewpointLocation + (Reach * PlayerViewpointRotation.Vector());
	DrawDebugLine(
		GetWorld(),
		PlayerViewpointLocation,
		LineTraceEnd,
		FColor(255,0,0),
		false,
		0.f,
		0.f,
		4.f
	);

	///Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// ray-cast out to reach distance (private var)
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// see what we hit
	AActor * ActorHit = Hit.GetActor();
	if (ActorHit) {
		// UE_LOG(LogTemp, Warning, TEXT("%s actor has been hit!"), *(ActorHit->GetName()));
	}
}

