// Copyright Alice Meng 2019

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	float OpenAngle = 0.f;

	UPROPERTY(EditAnywhere)
		float ClosedAngle = -90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume * PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.10f;

	float LastDoorOpenTime;

	// Find the owning Actor (ptr)
	AActor* Owner = nullptr;

	// Return total mass of actors on plate
	float GetTotalMassOfActorsOnPlate() const;
	
};
