// Fracture — Player Character
// Ada (AI) — Technical Director

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FractureCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UFractureHealthComponent;
class UFractureInventory;
class UFractureWeapon;
class UPostProcessComponent;
class USoundBase;
class UStaticMeshComponent;

UCLASS()
class FRACTURE_API AFractureCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFractureCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Input Mapping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	// Health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	TObjectPtr<UFractureHealthComponent> HealthComponent;



	// Combat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage = 35.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackCooldown = 0.8f;

	// True while attack animation is playing
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsAttacking = false;

	// Attack montage — assign in BP_FractureCharacter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<UAnimMontage> AttackMontage;

	// Roll montage — assign in BP_FractureCharacter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float RollDistance = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float RollCooldown = 0.8f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsRolling = false;

	// Movement settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 700.f;

	// Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenRate = 20.f; // per second

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float SprintStaminaCost = 25.f; // per second

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float RollStaminaCost = 20.f; // flat per roll

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stamina")
	float GetStaminaPercent() const { return MaxStamina > 0.f ? CurrentStamina / MaxStamina : 0.f; }

	// Sound effects — assign in BP_FractureCharacter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> RollSound;

	// Health percent exposed for Blueprint HUD
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

	// Scrap metal count for HUD
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetScrapMetal() const;





	// Post process for hit flash
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UPostProcessComponent> HitFlashPP;

	// Weapon held in hand
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UStaticMeshComponent> WeaponMeshComponent;

public:
	// Inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UFractureInventory> Inventory;

	// Show dialogue on HUD — called by NPC
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
	void ShowDialogue(const FString& SpeakerName, const FString& Line);

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FString CurrentSpeakerName;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FString CurrentDialogueLine;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	bool bDialogueActive = false;

	// Currently equipped weapon data
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UFractureWeapon> EquippedWeapon;

	// Equip a weapon from inventory
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapon(UFractureWeapon* Weapon);

	// Unequip current weapon
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UnequipWeapon();

protected:

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprint();
	void StopSprint();

	UFUNCTION()
	void OnDeath(AActor* DeadActor, AActor* Killer);

	UFUNCTION()
	void OnDamaged(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser);

	void Attack();
	void PerformAttackTrace();
	void ToggleCrouch();
	void Roll();
	void Interact();

	float LastAttackTime = -999.f;
	float LastRollTime = -999.f;
	float HitFlashTimer = 0.f;
	bool bIsSprinting = false;

	virtual void Tick(float DeltaTime) override;
};
