// Fracture — Hollow Knight AI Controller
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HollowKnightAIController.generated.h"

UCLASS()
class FRACTURE_API AHollowKnightAIController : public AAIController
{
	GENERATED_BODY()

public:
	AHollowKnightAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

private:
	// How often we update the chase target (seconds)
	float UpdateInterval = 0.2f;
	float TimeSinceLastUpdate = 0.f;

	void UpdateChase();
};
