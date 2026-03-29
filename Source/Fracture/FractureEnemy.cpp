// Fracture — Base Enemy Class
// Ada (AI) — Technical Director

#include "FractureEnemy.h"
#include "FractureHealthComponent.h"
#include "HollowKnightAIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/PointLightComponent.h"

AFractureEnemy::AFractureEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Health component
	HealthComponent = CreateDefaultSubobject<UFractureHealthComponent>(TEXT("HealthComponent"));

	// Corruption glow — deep red point light at chest height
	CorruptionLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CorruptionLight"));
	CorruptionLight->SetupAttachment(RootComponent);
	CorruptionLight->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	CorruptionLight->SetLightColor(FLinearColor(1.f, 0.05f, 0.f)); // deep red
	CorruptionLight->SetIntensity(3000.f);
	CorruptionLight->SetAttenuationRadius(200.f);
	CorruptionLight->SetCastShadows(false);

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

	// Pulse corruption glow
	if (CorruptionLight)
	{
		float Pulse = 0.7f + 0.3f * FMath::Sin(GetWorld()->GetTimeSeconds() * 3.f);
		CorruptionLight->SetIntensity(3000.f * Pulse * CorruptionGlowIntensity);
	}
}

void AFractureEnemy::TryAttack(AActor* Target)
{
	if (!Target) return;

	float Now = GetWorld()->GetTimeSeconds();
	if (Now - LastAttackTime < AttackCooldown) return;

	LastAttackTime = Now;

	// Play attack montage if assigned
	if (AttackMontage)
		PlayAnimMontage(AttackMontage);

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
