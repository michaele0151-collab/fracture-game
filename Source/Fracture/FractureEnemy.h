// Fracture — Base Enemy Class
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FractureEnemy.generated.h"

class UFractureHealthComponent;

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

	// Called by AI controller when in attack range
	void TryAttack(AActor* Target);

private:
	float LastAttackTime = -999.f;

	UFUNCTION()
	void OnDeath(AActor* DeadActor, AActor* Killer);
};
