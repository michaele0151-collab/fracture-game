// Fracture — HUD Widget
// Ada (AI) — Technical Director

#include "FractureHUD.h"
#include "FractureCharacter.h"
#include "FractureHealthComponent.h"
#include "Kismet/GameplayStatics.h"

void UFractureHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Sync health from player
	AFractureCharacter* Player = Cast<AFractureCharacter>(
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player)
	{
		UFractureHealthComponent* Health = Player->FindComponentByClass<UFractureHealthComponent>();
		if (Health)
			HealthPercent = Health->GetHealthPercent();
	}

	// Fade hit flash
	if (HitFlashTimer > 0.f)
	{
		HitFlashTimer -= InDeltaTime;
		HitFlashAlpha = FMath::Clamp(HitFlashTimer / HitFlashDuration, 0.f, 1.f);
	}
	else
	{
		HitFlashAlpha = 0.f;
	}
}

void UFractureHUD::TriggerHitFlash()
{
	HitFlashTimer = HitFlashDuration;
	HitFlashAlpha = 1.f;
}
