// Copyright Alice Meng 2019

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

}




// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	// Set the door rotation
	/*FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);*/
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	//UE_LOG(LogTemp, Warning, TEXT("Open Door rotation: %s!"), *(Owner->GetActorRotation().ToString()));
}

void UOpenDoor::CloseDoor()
{
	// Set the door rotation
	//FRotator NewRotation2 = FRotator(0.0f, 0.0f, 0.0f);
	Owner->SetActorRotation(FRotator(0.0f, ClosedAngle, 0.0f));  
	//UE_LOG(LogTemp, Warning, TEXT("closed Door   sdf rotation: %s!"), *(Owner->GetActorRotation().ToString()));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger location every frame
	if (GetTotalMassOfActorsOnPlate() > 50.f) {	// TODO make into a parameter
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}


	// Check if time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}
	

}

float UOpenDoor::GetTotalMassOfActorsOnPlate() const
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;

	// Get all overlapping actors
	PressurePlate->GetOverlappingActors(OUT OverlappingActors) ;

	// Sum up all masses
	for (const auto* OverlapActor : OverlappingActors) {
		TotalMass += OverlapActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	UE_LOG(LogTemp, Warning, TEXT("%f on the plate"), TotalMass);

	return TotalMass;
}

