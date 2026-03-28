// Fracture — Base Enemy Class
// Ada (AI) — Technical Director

#include "FractureEnemy.h"

AFractureEnemy::AFractureEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
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
