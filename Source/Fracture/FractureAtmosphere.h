// Fracture — Atmosphere Controller
// Handles post-process, fog density, and ambient mood
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FractureAtmosphere.generated.h"

class UPostProcessComponent;
class UExponentialHeightFogComponent;

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

	// Zone 1 mood: dark, cold, amber candlelight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
	float Exposure = -2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
	float Contrast = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
	FLinearColor ShadowTint = FLinearColor(0.02f, 0.02f, 0.06f, 1.f); // deep blue-black shadows

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
	FLinearColor MidtoneTint = FLinearColor(0.08f, 0.06f, 0.04f, 1.f); // desaturated amber midtones

private:
	void ApplyPostProcess();
};
