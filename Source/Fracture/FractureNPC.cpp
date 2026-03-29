// Fracture — NPC Actor
// Ada (AI) — Technical Director

#include "FractureNPC.h"
#include "FractureCharacter.h"

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
	if (DialogueLines.Num() == 0) return;

	FDialogueLine Line = GetCurrentLine();

	// Broadcast to player so HUD can display it
	AFractureCharacter* Player = Cast<AFractureCharacter>(Interactor);
	if (Player)
	{
		Player->ShowDialogue(NPCName, Line.Line);
	}

	// Advance to next line
	CurrentLineIndex = (CurrentLineIndex + 1) % DialogueLines.Num();
}

FDialogueLine AFractureNPC::GetCurrentLine() const
{
	if (DialogueLines.Num() == 0) return FDialogueLine();
	return DialogueLines[FMath::Clamp(CurrentLineIndex, 0, DialogueLines.Num() - 1)];
}
