// Fracture — HUD Widget
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FractureHUD.generated.h"

UCLASS()
class FRACTURE_API UFractureHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	// Called every frame to sync health from player
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Current health percent 0.0 - 1.0 — bind to progress bar in widget
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	float HealthPercent = 1.f;

	// Red flash intensity when hit
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	float HitFlashAlpha = 0.f;

	// Call this from FractureCharacter when damage is taken
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void TriggerHitFlash();

private:
	float HitFlashTimer = 0.f;
	static constexpr float HitFlashDuration = 0.3f;
};
