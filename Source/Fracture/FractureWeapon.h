// Fracture — Weapon Data Asset
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "FractureItem.h"
#include "FractureWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Knife		UMETA(DisplayName = "Knife"),
	Sword		UMETA(DisplayName = "Sword"),
	Axe			UMETA(DisplayName = "Axe"),
	Spear		UMETA(DisplayName = "Spear"),
	Crossbow	UMETA(DisplayName = "Crossbow"),
};

UCLASS(BlueprintType)
class FRACTURE_API UFractureWeapon : public UFractureItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::Knife;

	// Damage per hit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage = 15.f;

	// Attack range in units
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Range = 200.f;

	// Attack speed multiplier (1.0 = normal)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float AttackSpeed = 1.0f;

	// Stamina cost per swing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float StaminaCost = 10.f;

	// Mesh shown in hand when equipped
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UStaticMesh> WeaponMesh;

	// Attack montage to play
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UAnimMontage> AttackMontage;

	// Is this a corrupted weapon? (drains player over time)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsCorrupted = false;

	// Corruption drain per second when equipped
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float CorruptionDrainRate = 2.f;
};
