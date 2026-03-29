// Fracture — Hollowed AI Controller
// Ada (AI) — Technical Director

#include "HollowedAIController.h"
#include "HollowedEnemy.h"
#include "FractureCharacter.h"
#include "FractureHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AHollowedAIController::AHollowedAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHollowedAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ChaseAndAttack();
}

void AHollowedAIController::ChaseAndAttack()
{
	AHollowedEnemy* Hollowed = Cast<AHollowedEnemy>(GetPawn());
	if (!Hollowed) return;

	UFractureHealthComponent* MyHealth = Hollowed->HealthComponent;
	if (!MyHealth || MyHealth->bIsDead) return;

	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!Player) return;

	float Distance = FVector::Dist(Hollowed->GetActorLocation(), Player->GetActorLocation());

	// Detection — faster if player is sprinting (velocity check)
	float DetectRange = Hollowed->DetectionRange;
	float PlayerSpeed = Player->GetVelocity().Size();
	if (PlayerSpeed > 500.f) // sprinting threshold
		DetectRange *= Hollowed->SprintDetectionMultiplier;

	if (Distance <= DetectRange)
	{
		if (Distance > 120.f)
		{
			// Chase — Hollowed are relentless, no slowing
			MoveToActor(Player, 80.f);
		}
		else
		{
			StopMovement();
			Hollowed->TryAttack(Player);
		}
	}
	else
	{
		StopMovement();
	}
}
