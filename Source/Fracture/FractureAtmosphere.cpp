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

	// Contrast
	PPSettings.bOverride_FilmContrast = true;
	PPSettings.FilmContrast = Contrast;

	// Shadow tint — deep blue-black
	PPSettings.bOverride_ColorShadows = true;
	PPSettings.ColorShadows = FVector4(ShadowTint.R, ShadowTint.G, ShadowTint.B, 1.f);

	// Midtone tint — desaturated amber
	PPSettings.bOverride_ColorMidtones = true;
	PPSettings.ColorMidtones = FVector4(MidtoneTint.R, MidtoneTint.G, MidtoneTint.B, 1.f);

	// Desaturate slightly — colours are muted in Zone 1
	PPSettings.bOverride_FilmSaturation = true;
	PPSettings.FilmSaturation = 0.7f;

	// Vignette — dark edges
	PPSettings.bOverride_VignetteIntensity = true;
	PPSettings.VignetteIntensity = 0.6f;

	// Slight grain — gritty feel
	PPSettings.bOverride_GrainIntensity = true;
	PPSettings.GrainIntensity = 0.3f;

	UE_LOG(LogTemp, Warning, TEXT("FractureAtmosphere: Zone 1 mood applied."));
}
