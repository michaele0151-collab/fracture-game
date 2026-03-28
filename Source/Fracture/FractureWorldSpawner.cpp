// Fracture — Procedural World Spawner
// Ada (AI) — Technical Director

#include "FractureWorldSpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"

AFractureWorldSpawner::AFractureWorldSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFractureWorldSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnObjects();
}

void AFractureWorldSpawner::SpawnObjects()
{
	if (SpawnEntries.Num() == 0) return;

	FRandomStream Rand(Seed);

	for (const FSpawnEntry& Entry : SpawnEntries)
	{
		if (!Entry.Mesh) continue;

		int32 Spawned = 0;
		int32 Attempts = 0;
		int32 MaxAttempts = Entry.Count * 10;

		while (Spawned < Entry.Count && Attempts < MaxAttempts)
		{
			Attempts++;
			FVector SpawnLoc = FindSpawnLocation(Rand);

			// Skip if too close to center (player spawn area)
			if (SpawnLoc.Size2D() < ClearRadiusAroundCenter) continue;

			// Trace down to find ground
			FHitResult Hit;
			FVector TraceStart = SpawnLoc + FVector(0, 0, 500.f);
			FVector TraceEnd = SpawnLoc - FVector(0, 0, 1000.f);

			FCollisionQueryParams Params;
			Params.bTraceComplex = false;

			if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldStatic, Params))
			{
				FVector FinalLoc = Hit.Location;

				// Random scale
				float Scale = Rand.FRandRange(Entry.MinScale, Entry.MaxScale);

				// Random yaw rotation
				FRotator Rotation(0.f, Rand.FRandRange(0.f, 360.f), 0.f);

				// Spawn the mesh actor
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AStaticMeshActor* NewActor = GetWorld()->SpawnActor<AStaticMeshActor>(
					AStaticMeshActor::StaticClass(), FinalLoc, Rotation, SpawnParams);

				if (NewActor)
				{
					NewActor->GetStaticMeshComponent()->SetStaticMesh(Entry.Mesh);
					NewActor->SetActorScale3D(FVector(Scale));
					NewActor->SetMobility(EComponentMobility::Static);
					Spawned++;
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("FractureWorldSpawner: Spawned %d/%d of %s"),
			Spawned, Entry.Count, *Entry.Mesh->GetName());
	}
}

FVector AFractureWorldSpawner::FindSpawnLocation(FRandomStream& Rand)
{
	FVector Origin = GetActorLocation();
	float Angle = Rand.FRandRange(0.f, 2.f * PI);
	float Distance = Rand.FRandRange(0.f, SpawnRadius);
	return FVector(
		Origin.X + FMath::Cos(Angle) * Distance,
		Origin.Y + FMath::Sin(Angle) * Distance,
		Origin.Z
	);
}
