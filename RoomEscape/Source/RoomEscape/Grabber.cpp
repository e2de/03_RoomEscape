// Copyright Alice Meng 2019

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Public/DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"


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
	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("%s has GRABBED"), *(GetOwner()->GetName()));

	// Try and reach any accors with physics body collision channel sel
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit) {
		// if we hid something then attach a physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation!
		);
	}



}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("%s has RELEASED"), *(GetOwner()->GetName()));
	//TODO release physics handle
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->ReleaseComponent();
	}
}

// Finding PhysicsHandleComponent
void UGrabber::FindPhysicsHandleComponent (){

	/// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle) {
		// Physics handle is found
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s is missing Physics Handle component"), *(GetOwner()->GetName()));
	}
}

void UGrabber::SetupInputComponent()
{
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

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation);

	FVector UnitRotation;
	PlayerViewpointRotation.Quaternion().Vector();
	FVector LineTraceEnd = PlayerViewpointLocation + (Reach * PlayerViewpointRotation.Vector());

	DrawDebugLine(
		GetWorld(),
		PlayerViewpointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		1.f
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
		 UE_LOG(LogTemp, Warning, TEXT("%s actor has been hit!"), *(ActorHit->GetName()));
	}
	return Hit;
}
			

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation);

	FVector UnitRotation;
	PlayerViewpointRotation.Quaternion().Vector();
	FVector LineTraceEnd = PlayerViewpointLocation + (Reach * PlayerViewpointRotation.Vector());


	// if physics handle attached
	if (PhysicsHandle->GrabbedComponent) {
		// move obj holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

	
	
}

