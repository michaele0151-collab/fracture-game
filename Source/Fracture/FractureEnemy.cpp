// Fracture — Base Enemy Class
// Ada (AI) — Technical Director

#include "FractureEnemy.h"
#include "FractureHealthComponent.h"
#include "HollowKnightAIController.h"

AFractureEnemy::AFractureEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Health component
	HealthComponent = CreateDefaultSubobject<UFractureHealthComponent>(TEXT("HealthComponent"));

	// Auto-assign the AI controller
	AIControllerClass = AHollowKnightAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AFractureEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &AFractureEnemy::OnDeath);
	}
}

void AFractureEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFractureEnemy::TryAttack(AActor* Target)
{
	if (!Target) return;

	float Now = GetWorld()->GetTimeSeconds();
	if (Now - LastAttackTime < AttackCooldown) return;

	LastAttackTime = Now;

	// Find health component on target and deal damage
	UFractureHealthComponent* TargetHealth = Target->FindComponentByClass<UFractureHealthComponent>();
	if (TargetHealth)
	{
		TargetHealth->TakeDamage(AttackDamage, this);
		UE_LOG(LogTemp, Warning, TEXT("Hollow Knight attacks %s for %.1f damage"), *Target->GetName(), AttackDamage);
	}
}

void AFractureEnemy::OnDeath(AActor* DeadActor, AActor* Killer)
{
	// Ragdoll
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Destroy after 5 seconds
	SetLifeSpan(5.f);
}
