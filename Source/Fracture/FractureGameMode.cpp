// Fracture — Game Mode
// Ada (AI) — Technical Director

#include "FractureGameMode.h"
#include "FractureCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFractureGameMode::AFractureGameMode()
{
	// Default pawn is our FractureCharacter
	// Blueprint subclass (BP_FractureCharacter) will override this at runtime
	// Set in Project Settings → Maps & Modes after Blueprint is created
}
