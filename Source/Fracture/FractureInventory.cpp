// Fracture — Inventory Component
// Ada (AI) — Technical Director

#include "FractureInventory.h"

UFractureInventory::UFractureInventory()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UFractureInventory::AddItem(UFractureItem* Item, int32 Quantity)
{
	if (!Item || Quantity <= 0) return false;

	// Check weight
	if (GetCurrentWeight() + (Item->Weight * Quantity) > MaxWeight)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory full — cannot add %s"), *Item->DisplayName.ToString());
		return false;
	}

	// Currency goes to scrap pool directly
	if (Item->ItemType == EItemType::Currency)
	{
		AddScrap(Quantity);
		return true;
	}

	// Try to stack into existing slot
	int32 ExistingIdx = FindSlot(Item);
	if (ExistingIdx != INDEX_NONE && Item->MaxStackSize > 1)
	{
		Slots[ExistingIdx].Quantity += Quantity;
		OnInventoryChanged.Broadcast();
		return true;
	}

	// New slot
	FInventorySlot NewSlot;
	NewSlot.Item = Item;
	NewSlot.Quantity = Quantity;
	Slots.Add(NewSlot);
	OnInventoryChanged.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("Picked up: %s x%d"), *Item->DisplayName.ToString(), Quantity);
	return true;
}

bool UFractureInventory::RemoveItem(UFractureItem* Item, int32 Quantity)
{
	int32 Idx = FindSlot(Item);
	if (Idx == INDEX_NONE) return false;

	Slots[Idx].Quantity -= Quantity;
	if (Slots[Idx].Quantity <= 0)
		Slots.RemoveAt(Idx);

	OnInventoryChanged.Broadcast();
	return true;
}

bool UFractureInventory::HasItem(UFractureItem* Item, int32 Quantity) const
{
	int32 Idx = FindSlot(Item);
	if (Idx == INDEX_NONE) return false;
	return Slots[Idx].Quantity >= Quantity;
}

void UFractureInventory::AddScrap(int32 Amount)
{
	ScrapMetal += Amount;
	OnInventoryChanged.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Scrap: +%d (Total: %d)"), Amount, ScrapMetal);
}

bool UFractureInventory::SpendScrap(int32 Amount)
{
	if (ScrapMetal < Amount) return false;
	ScrapMetal -= Amount;
	OnInventoryChanged.Broadcast();
	return true;
}

float UFractureInventory::GetCurrentWeight() const
{
	float Total = 0.f;
	for (const FInventorySlot& Slot : Slots)
	{
		if (Slot.Item)
			Total += Slot.Item->Weight * Slot.Quantity;
	}
	return Total;
}

int32 UFractureInventory::FindSlot(UFractureItem* Item) const
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i].Item == Item)
			return i;
	}
	return INDEX_NONE;
}
