// Fracture — The Hollowed Enemy
// Ada (AI) — Technical Director
// Fast, feral, hunts by movement. Zone 1 common enemy.

#pragma once

#include "CoreMinimal.h"
#include "FractureEnemy.h"
#include "HollowedEnemy.generated.h"

UCLASS()
class FRACTURE_API AHollowedEnemy : public AFractureEnemy
{
	GENERATED_BODY()

public:
	AHollowedEnemy();

	// Detection is faster when player is sprinting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float SprintDetectionMultiplier = 2.5f;

	// Hollowed are faster but weaker
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float ChaseSpeed = 500.f;

protected:
	virtual void BeginPlay() override;
};
