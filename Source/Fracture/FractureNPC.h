// Fracture — NPC Actor
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FractureNPC.generated.h"

USTRUCT(BlueprintType)
struct FDialogueLine
{
	GENERATED_BODY()

	// The line of dialogue
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Line;

	// Optional: only show this line if a quest is active (leave blank = always show)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName RequiredQuestID;

	// Optional: only show if quest is complete
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName RequiredCompletedQuestID;
};

UCLASS()
class FRACTURE_API AFractureNPC : public ACharacter
{
	GENERATED_BODY()

public:
	AFractureNPC();

	// NPC name shown above dialogue
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText NPCName;

	// All dialogue lines — cycles through them on repeated interact
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FDialogueLine> DialogueLines;

	// Does this NPC open a vendor when interacted with?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	bool bIsVendor = false;

	// Does this NPC give a quest?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName QuestToGive;

	// Called by player interact
	void Interact(AActor* Interactor);

	// Get current dialogue line (cycles)
	FDialogueLine GetCurrentLine() const;

protected:
	virtual void BeginPlay() override;

private:
	int32 CurrentLineIndex = 0;
};
