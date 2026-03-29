// Fracture — Health Component
// Shared by player and enemies
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FractureHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeath, AActor*, DeadActor, AActor*, Killer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamaged, AActor*, DamagedActor, float, DamageAmount, AActor*, DamageCauser);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FRACTURE_API UFractureHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFractureHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bIsDead = false;

	// Invincibility frames — set true during roll
	UPROPERTY(BlueprintReadWrite, Category = "Health")
	bool bInvincible = false;

	// Called when damage is taken
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDamaged OnDamaged;

	// Called on death
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float Amount, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const;

protected:
	virtual void BeginPlay() override;
};
