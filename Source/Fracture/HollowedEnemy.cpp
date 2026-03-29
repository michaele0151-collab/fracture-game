// Fracture — The Hollowed Enemy
// Ada (AI) — Technical Director

#include "HollowedEnemy.h"
#include "FractureHealthComponent.h"
#include "HollowedAIController.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AHollowedEnemy::AHollowedEnemy()
{
	// Weaker and faster than the Hollow Knight
	if (HealthComponent)
		HealthComponent->MaxHealth = 40.f;

	AttackDamage = 10.f;
	AttackCooldown = 1.0f;
	DetectionRange = 1200.f;
	ScrapDrop = 5;

	// Dimmer, more sickly glow than Hollow Knight
	if (CorruptionLight)
	{
		CorruptionLight->SetLightColor(FLinearColor(0.8f, 0.f, 0.1f));
		CorruptionLight->SetIntensity(1200.f);
		CorruptionLight->SetAttenuationRadius(120.f);
	}

	// Use Hollowed AI controller
	AIControllerClass = AHollowedAIController::StaticClass();
}

void AHollowedEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
}
