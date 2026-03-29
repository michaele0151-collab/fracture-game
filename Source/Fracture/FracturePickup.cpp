// Fracture — World Pickup Actor
// Ada (AI) — Technical Director

#include "FracturePickup.h"
#include "FractureItem.h"
#include "FractureInventory.h"
#include "FractureWeapon.h"
#include "FractureCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AFracturePickup::AFracturePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
	InteractSphere->SetSphereRadius(100.f);
	InteractSphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = InteractSphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFracturePickup::BeginPlay()
{
	Super::BeginPlay();
	BaseLocation = GetActorLocation();
}

void AFracturePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Gentle hover bob
	HoverOffset += DeltaTime * 2.f;
	FVector NewLoc = BaseLocation;
	NewLoc.Z += FMath::Sin(HoverOffset) * 8.f;
	SetActorLocation(NewLoc);
}

void AFracturePickup::Interact(AActor* Interactor)
{
	if (!Item || !Interactor) return;

	UFractureInventory* Inventory = Interactor->FindComponentByClass<UFractureInventory>();
	if (!Inventory) return;

	if (Inventory->AddItem(Item, Quantity))
	{
		// Auto-equip weapons on pickup if nothing equipped
		UFractureWeapon* WeaponItem = Cast<UFractureWeapon>(Item);
		AFractureCharacter* Character = Cast<AFractureCharacter>(Interactor);
		if (WeaponItem && Character && !Character->EquippedWeapon)
			Character->EquipWeapon(WeaponItem);

		Destroy();
	}
}
