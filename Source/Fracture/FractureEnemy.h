// Fracture — Base Enemy Class
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FractureEnemy.generated.h"

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

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float CurrentHealth;

	// Detection range — how far the enemy can sense the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DetectionRange = 1500.f;

	// Whether this enemy is currently aware of the player
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool bIsAware = false;

	// Hollow corruption glow intensity (0-1), used by material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	float CorruptionGlowIntensity = 1.f;
};
