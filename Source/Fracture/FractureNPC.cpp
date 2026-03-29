// Fracture — NPC Actor
// Ada (AI) — Technical Director

#include "FractureNPC.h"
#include "FractureCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AFractureNPC::AFractureNPC()
{
	PrimaryActorTick.bCanEverTick = false;

	// NPCs don't move
	GetCharacterMovement()->DisableMovement();
}

void AFractureNPC::BeginPlay()
{
	Super::BeginPlay();
}

void AFractureNPC::Interact(AActor* Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("NPC Interact called"));

	if (DialogueLines.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NPC has no dialogue lines"));
		return;
	}

	FDialogueLine Line = GetCurrentLine();
	UE_LOG(LogTemp, Warning, TEXT("NPC says: %s"), *Line.Line);

	AFractureCharacter* Player = Cast<AFractureCharacter>(Interactor);
	if (Player)
	{
		Player->ShowDialogue(NPCName, Line.Line);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Interactor is not FractureCharacter"));
	}

	CurrentLineIndex = (CurrentLineIndex + 1) % DialogueLines.Num();
}

FDialogueLine AFractureNPC::GetCurrentLine() const
{
	if (DialogueLines.Num() == 0) return FDialogueLine();
	return DialogueLines[FMath::Clamp(CurrentLineIndex, 0, DialogueLines.Num() - 1)];
}
