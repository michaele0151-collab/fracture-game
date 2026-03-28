// Fracture — Health Component
// Ada (AI) — Technical Director

#include "FractureHealthComponent.h"

UFractureHealthComponent::UFractureHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFractureHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void UFractureHealthComponent::TakeDamage(float Amount, AActor* DamageCauser)
{
	if (bIsDead || Amount <= 0.f) return;

	CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0.f, MaxHealth);
	OnDamaged.Broadcast(GetOwner(), Amount, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("%s took %.1f damage — Health: %.1f/%.1f"),
		*GetOwner()->GetName(), Amount, CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.f)
	{
		bIsDead = true;
		OnDeath.Broadcast(GetOwner(), DamageCauser);
		UE_LOG(LogTemp, Warning, TEXT("%s died."), *GetOwner()->GetName());
	}
}

float UFractureHealthComponent::GetHealthPercent() const
{
	return MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f;
}
