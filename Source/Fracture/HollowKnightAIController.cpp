// Fracture — Hollow Knight AI Controller
// Ada (AI) — Technical Director

#include "HollowKnightAIController.h"
#include "FractureEnemy.h"
#include "FractureCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

AHollowKnightAIController::AHollowKnightAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHollowKnightAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AHollowKnightAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastUpdate += DeltaTime;
	if (TimeSinceLastUpdate >= UpdateInterval)
	{
		TimeSinceLastUpdate = 0.f;
		UpdateChase();
	}
}

void AHollowKnightAIController::UpdateChase()
{
	AFractureEnemy* Enemy = Cast<AFractureEnemy>(GetPawn());
	if (!Enemy) return;

	// Find the player
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Player) return;

	float Distance = FVector::Dist(Enemy->GetActorLocation(), Player->GetActorLocation());

	UE_LOG(LogTemp, Warning, TEXT("HollowKnight distance to player: %.1f"), Distance);

	if (Distance <= Enemy->DetectionRange)
	{
		// Become aware
		Enemy->bIsAware = true;

		if (Distance <= 200.f)
		{
			// In attack range — stop and attack
			StopMovement();
			UE_LOG(LogTemp, Warning, TEXT("HollowKnight in attack range — attacking!"));
			Enemy->TryAttack(Player);
		}
		else
		{
			// Chase the player
			MoveToActor(Player, 150.f);
		}
	}
	else
	{
		// Lost the player
		Enemy->bIsAware = false;
		StopMovement();
	}
}
