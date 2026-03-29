// Fracture — World Pickup Actor
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FracturePickup.generated.h"

class UFractureItem;
class UStaticMeshComponent;
class USphereComponent;
class UWidgetComponent;

UCLASS()
class FRACTURE_API AFracturePickup : public AActor
{
	GENERATED_BODY()

public:
	AFracturePickup();

	// The item this pickup represents
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	TObjectPtr<UFractureItem> Item;

	// How many of this item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	int32 Quantity = 1;

	// Mesh displayed in world
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	// Interact radius
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> InteractSphere;

	// Called by the player's interact trace
	void Interact(AActor* Interactor);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	float HoverOffset = 0.f;
	FVector BaseLocation;
};
