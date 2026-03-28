// Fracture — Procedural World Spawner
// Drag one of these into the level — it populates the world automatically
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FractureWorldSpawner.generated.h"

USTRUCT(BlueprintType)
struct FSpawnEntry
{
	GENERATED_BODY()

	// The static mesh to spawn (assign in Blueprint)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TObjectPtr<UStaticMesh> Mesh;

	// How many to scatter across the level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 Count = 20;

	// Min/max scale variation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float MinScale = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float MaxScale = 1.4f;
};

UCLASS()
class FRACTURE_API AFractureWorldSpawner : public AActor
{
	GENERATED_BODY()

public:
	AFractureWorldSpawner();

protected:
	virtual void BeginPlay() override;

public:
	// List of meshes to scatter — assign in Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<FSpawnEntry> SpawnEntries;

	// Radius around this actor to scatter objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnRadius = 4000.f;

	// Minimum distance between spawned objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float MinSpacing = 200.f;

	// Keep this area clear around the player start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float ClearRadiusAroundCenter = 500.f;

	// Random seed — change for different layouts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 Seed = 42;

private:
	void SpawnObjects();
	FVector FindSpawnLocation(FRandomStream& Rand);
};
