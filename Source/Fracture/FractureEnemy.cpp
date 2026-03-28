// Fracture — Base Enemy Class
// Ada (AI) — Technical Director

#include "FractureEnemy.h"
#include "HollowKnightAIController.h"

AFractureEnemy::AFractureEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Auto-assign the AI controller
	AIControllerClass = AHollowKnightAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AFractureEnemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void AFractureEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
