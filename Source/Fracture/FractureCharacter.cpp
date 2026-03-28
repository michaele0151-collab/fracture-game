// Fracture — Player Character
// Ada (AI) — Technical Director

#include "FractureCharacter.h"
#include "FractureHealthComponent.h"
#include "FractureEnemy.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PostProcessComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AFractureCharacter::AFractureCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Camera boom (third person)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 8.f;
	CameraBoom->CameraLagMaxDistance = 80.f;

	// Follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Movement — weighty feel
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->GravityScale = 1.5f;

	// Don't rotate with camera — movement handles rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Health component
	HealthComponent = CreateDefaultSubobject<UFractureHealthComponent>(TEXT("HealthComponent"));

	// Hit flash post process
	HitFlashPP = CreateDefaultSubobject<UPostProcessComponent>(TEXT("HitFlashPP"));
	HitFlashPP->SetupAttachment(RootComponent);
	HitFlashPP->bUnbound = true;
	HitFlashPP->BlendWeight = 1.f;
	HitFlashPP->Settings.bOverride_SceneColorTint = true;
	HitFlashPP->Settings.SceneColorTint = FLinearColor(1.f, 1.f, 1.f); // neutral by default
}

void AFractureCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Lock input to game — capture mouse, hide cursor
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);
		PC->bEnableClickEvents = false;

		// Force mouse capture on the viewport
		if (PC->GetLocalPlayer() && PC->GetLocalPlayer()->ViewportClient)
		{
			PC->GetLocalPlayer()->ViewportClient->SetMouseCaptureMode(EMouseCaptureMode::CapturePermanently);
		}
	}

	// Hook up death + damage flash
	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &AFractureCharacter::OnDeath);
		HealthComponent->OnDamaged.AddDynamic(this, &AFractureCharacter::OnDamaged);
	}
}

void AFractureCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Load input assets at binding time — most reliable approach
	if (!DefaultMappingContext)
		DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Input/IMC_Default"));
	if (!MoveAction)
		MoveAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/IA_Move"));
	if (!LookAction)
		LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/IA_Look"));
	if (!JumpAction)
		JumpAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/IA_Jump"));
	if (!SprintAction)
		SprintAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/IA_Sprint"));
	if (!AttackAction)
		AttackAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/IA_Attack"));

	// Add mapping context
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Bind actions
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFractureCharacter::Move);
		if (LookAction)
			EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFractureCharacter::Look);
		if (JumpAction)
		{
			EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}
		if (SprintAction)
		{
			EIC->BindAction(SprintAction, ETriggerEvent::Started, this, &AFractureCharacter::StartSprint);
			EIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &AFractureCharacter::StopSprint);
		}
		if (AttackAction)
			EIC->BindAction(AttackAction, ETriggerEvent::Started, this, &AFractureCharacter::Attack);
	}
}

void AFractureCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFractureCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFractureCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AFractureCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFractureCharacter::Attack()
{
	float Now = GetWorld()->GetTimeSeconds();
	if (Now - LastAttackTime < AttackCooldown) return;
	if (bIsAttacking) return;

	LastAttackTime = Now;
	bIsAttacking = true;

	// Play attack montage if assigned
	if (AttackMontage)
	{
		float Duration = PlayAnimMontage(AttackMontage);
		// Reset attack flag after montage
		FTimerHandle AttackTimer;
		GetWorldTimerManager().SetTimer(AttackTimer, [this]()
		{
			bIsAttacking = false;
		}, FMath::Max(Duration, 0.3f), false);
	}
	else
	{
		// No montage — instant attack trace
		PerformAttackTrace();
		FTimerHandle AttackTimer;
		GetWorldTimerManager().SetTimer(AttackTimer, [this]()
		{
			bIsAttacking = false;
		}, 0.3f, false);
	}

	// Perform the damage trace
	PerformAttackTrace();
}

void AFractureCharacter::PerformAttackTrace()
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * AttackRange;

	// Sphere sweep — forgiving hitbox
	TArray<FHitResult> Hits;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(80.f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Pawn, Sphere, Params);

	// Debug — show attack range
	DrawDebugSphere(GetWorld(), End, 80.f, 8, FColor::Red, false, 0.5f);

	for (FHitResult& Hit : Hits)
	{
		AFractureEnemy* Enemy = Cast<AFractureEnemy>(Hit.GetActor());
		if (Enemy)
		{
			UFractureHealthComponent* EnemyHealth = Enemy->FindComponentByClass<UFractureHealthComponent>();
			if (EnemyHealth && !EnemyHealth->bIsDead)
			{
				EnemyHealth->TakeDamage(AttackDamage, this);
				UE_LOG(LogTemp, Warning, TEXT("Player hit %s for %.1f"), *Enemy->GetName(), AttackDamage);
			}
		}
	}
}

float AFractureCharacter::GetHealthPercent() const
{
	if (HealthComponent)
		return HealthComponent->GetHealthPercent();
	return 1.f;
}

void AFractureCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Fade hit flash out over time
	if (HitFlashTimer > 0.f)
	{
		HitFlashTimer -= DeltaTime;
		float Alpha = FMath::Clamp(HitFlashTimer / 0.3f, 0.f, 1.f);
		if (HitFlashPP)
			HitFlashPP->Settings.SceneColorTint = FLinearColor(1.f, FMath::Lerp(1.f, 0.1f, Alpha), FMath::Lerp(1.f, 0.1f, Alpha));
	}
	else
	{
		if (HitFlashPP)
			HitFlashPP->Settings.SceneColorTint = FLinearColor(1.f, 1.f, 1.f);
	}
}

void AFractureCharacter::OnDamaged(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser)
{
	HitFlashTimer = 0.3f;
}

void AFractureCharacter::OnDeath(AActor* DeadActor, AActor* Killer)
{
	// Disable input
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}

	// Stop movement on death
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	UE_LOG(LogTemp, Warning, TEXT("Player died. Respawning in 3 seconds..."));

	// Respawn after 3 seconds
	FTimerHandle RespawnTimer;
	GetWorldTimerManager().SetTimer(RespawnTimer, [this]()
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			PC->RestartLevel();
		}
	}, 3.f, false);
}
