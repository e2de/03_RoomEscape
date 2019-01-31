// Copyright Alice Meng 2019
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called when the game starts
	virtual void BeginPlay() override;

	
private:
	// How far ahead of the player can we reach? in cm
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
		
	// Input Handler
	UInputComponent * InputHandle = nullptr;

	// Ray casts and grabs what's in reach
	void Grab();

	// Called when grab is released
	void Release();

	// Finding PhysicsHandleComponent
	void FindPhysicsHandleComponent();

	// Setup (assumed) attached Input Component
	void SetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// Returns current start of reach line 
	const FVector GetReachLineStart();

	// Returns current end of reach line 
	const FVector GetReachLineEnd();
};
