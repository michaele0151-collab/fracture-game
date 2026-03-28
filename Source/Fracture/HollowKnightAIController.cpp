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

	if (Distance <= Enemy->DetectionRange)
	{
		// Become aware
		Enemy->bIsAware = true;

		// Chase the player
		MoveToActor(Player, 150.f); // stop 150 units away (attack range)
	}
	else
	{
		// Lost the player
		Enemy->bIsAware = false;
		StopMovement();
	}
}
