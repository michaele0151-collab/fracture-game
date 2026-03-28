// Fracture — Player Character
// Ada (AI) — Technical Director

#include "FractureCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
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
