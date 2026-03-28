// Fracture — Atmosphere Controller
// Ada (AI) — Technical Director

#include "FractureAtmosphere.h"
#include "Components/PostProcessComponent.h"

AFractureAtmosphere::AFractureAtmosphere()
{
	PrimaryActorTick.bCanEverTick = false;

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	RootComponent = PostProcess;

	// Infinite extent — affects whole world
	PostProcess->bUnbound = true;
}

void AFractureAtmosphere::BeginPlay()
{
	Super::BeginPlay();
	ApplyPostProcess();
}

void AFractureAtmosphere::ApplyPostProcess()
{
	FPostProcessSettings& PPSettings = PostProcess->Settings;

	// Exposure — make it dark
	PPSettings.bOverride_AutoExposureBias = true;
	PPSettings.AutoExposureBias = Exposure;

	// Vignette — dark edges
	PPSettings.bOverride_VignetteIntensity = true;
	PPSettings.VignetteIntensity = 0.6f;

	// Color grading — Zone 1 cold dark mood
	// Overall scene tint via color grading gain (shadows)
	PPSettings.bOverride_ColorGradingIntensity = true;
	PPSettings.ColorGradingIntensity = 1.0f;

	// Scene color tint — desaturated cold blue
	PPSettings.bOverride_SceneColorTint = true;
	PPSettings.SceneColorTint = FLinearColor(0.6f, 0.65f, 0.8f); // cold blue-grey tint

	// Bloom — subtle atmospheric glow
	PPSettings.bOverride_BloomIntensity = true;
	PPSettings.BloomIntensity = 0.3f;

	// Ambient occlusion — deep shadows in crevices
	PPSettings.bOverride_AmbientOcclusionIntensity = true;
	PPSettings.AmbientOcclusionIntensity = 1.0f;

	PPSettings.bOverride_AmbientOcclusionRadius = true;
	PPSettings.AmbientOcclusionRadius = 200.f;

	UE_LOG(LogTemp, Warning, TEXT("FractureAtmosphere: Zone 1 mood applied."));
}
