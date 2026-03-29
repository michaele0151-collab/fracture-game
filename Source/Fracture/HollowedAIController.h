// Fracture — Hollowed AI Controller
// Ada (AI) — Technical Director
// Aggressive, fast, minimal stop distance. Hunts by movement.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HollowedAIController.generated.h"

UCLASS()
class FRACTURE_API AHollowedAIController : public AAIController
{
	GENERATED_BODY()

public:
	AHollowedAIController();

	virtual void Tick(float DeltaTime) override;

private:
	void ChaseAndAttack();
};
