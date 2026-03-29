// Fracture — Inventory Component
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FractureItem.h"
#include "FractureInventory.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFractureItem> Item = nullptr;

	UPROPERTY(BlueprintReadOnly)
	int32 Quantity = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FRACTURE_API UFractureInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UFractureInventory();

	// All items in the inventory
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> Slots;

	// Scrap metal currency
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 ScrapMetal = 0;

	// Max weight player can carry
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float MaxWeight = 50.f;

	// Fired whenever inventory changes — Blueprint HUD binds to this
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

	// Add an item — returns true if successful
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UFractureItem* Item, int32 Quantity = 1);

	// Remove an item — returns true if found and removed
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(UFractureItem* Item, int32 Quantity = 1);

	// Check if inventory contains an item
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool HasItem(UFractureItem* Item, int32 Quantity = 1) const;

	// Add scrap metal
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddScrap(int32 Amount);

	// Spend scrap metal — returns false if insufficient
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool SpendScrap(int32 Amount);

	// Current total weight
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	float GetCurrentWeight() const;

private:
	// Find existing slot for item — returns INDEX_NONE if not found
	int32 FindSlot(UFractureItem* Item) const;
};
