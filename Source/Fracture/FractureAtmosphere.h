// Fracture — Atmosphere Controller
// Handles post-process and ambient mood
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FractureAtmosphere.generated.h"

class UPostProcessComponent;

UCLASS()
class FRACTURE_API AFractureAtmosphere : public AActor
{
	GENERATED_BODY()

public:
	AFractureAtmosphere();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPostProcessComponent> PostProcess;

	// Zone 1 mood: dark exposure
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
	float Exposure = -2.f;

private:
	void ApplyPostProcess();
};
