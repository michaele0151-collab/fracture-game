// Fracture — Base Item Data
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FractureItem.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon		UMETA(DisplayName = "Weapon"),
	Armour		UMETA(DisplayName = "Armour"),
	Consumable	UMETA(DisplayName = "Consumable"),
	Currency	UMETA(DisplayName = "Currency"),
	Lore		UMETA(DisplayName = "Lore"),
	Quest		UMETA(DisplayName = "Quest"),
};

UCLASS(BlueprintType)
class FRACTURE_API UFractureItem : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	EItemType ItemType = EItemType::Consumable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	float Weight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 MaxStackSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 VendorPrice = 10; // cost in ScrapMetal

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UTexture2D> Icon;
};
