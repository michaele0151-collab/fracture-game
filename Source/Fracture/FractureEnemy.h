// Fracture — Base Enemy Class
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FractureEnemy.generated.h"

class UFractureHealthComponent;
class UPointLightComponent;
class USoundBase;
class UFractureItem;
class AFracturePickup;

UCLASS()
class FRACTURE_API AFractureEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AFractureEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Health component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	TObjectPtr<UFractureHealthComponent> HealthComponent;

	// Corruption glow light
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	TObjectPtr<UPointLightComponent> CorruptionLight;

	// Detection range — how far the enemy can sense the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DetectionRange = 1500.f;

	// Whether this enemy is currently aware of the player
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool bIsAware = false;

	// Attack damage per hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage = 20.f;

	// Attack cooldown in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackCooldown = 1.5f;

	// Hollow corruption glow intensity (0-1), used by material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	float CorruptionGlowIntensity = 1.f;

	// Attack montage — assign in BP_HollowKnight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<UAnimMontage> AttackMontage;

	// Sound effects — assign in BP_HollowKnight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> DeathSound;

	// Scrap dropped on death
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	int32 ScrapDrop = 10;

	// Optional item drop on death — assign in BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TObjectPtr<UFractureItem> LootItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	float LootDropChance = 0.3f; // 30% chance

	// Called by AI controller when in attack range
	void TryAttack(AActor* Target);

private:
	float LastAttackTime = -999.f;

	UFUNCTION()
	void OnDeath(AActor* DeadActor, AActor* Killer);
};
