// Copyright (c) 2025 Filip Dunal. All Rights Reserved.


#include "ProjectStealth/Gameplay/Characters/PSPlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProjectStealth/Gameplay/Components/PSInteractionComponent.h"


APSPlayerCharacter::APSPlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	InteractionComponent = CreateDefaultSubobject<UPSInteractionComponent>(TEXT("Interaction Component"));
	InteractionComponent->SetupAttachment(RootComponent);

	LookAccumulatedValue = FVector2D(0.0f, 1.0f);
	LookSmoothing = 0.1f;
}


void APSPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}


void APSPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsValid(Controller))
	{
		const FVector RotVector = FVector(LookAccumulatedValue.Y, LookAccumulatedValue.X, 0.0f);
		const FRotator TargetControlRotation = FRotationMatrix::MakeFromX(RotVector).Rotator();
		const FRotator CurrentControlRotation = Controller->GetControlRotation();

		Controller->SetControlRotation(FMath::RInterpTo(CurrentControlRotation, TargetControlRotation, DeltaTime, LookSmoothing * LookAccumulatedValue.Length()));
	}
}


void APSPlayerCharacter::NotifyControllerChanged()
{
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		Subsystem->AddMappingContext(DefaultInputMapping, 0);
	}
}

void APSPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &APSPlayerCharacter::Move);
		EnhancedInput->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &APSPlayerCharacter::Look);
	}
}


void APSPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector MoveVector = FVector(Value.Get<FVector2D>().Y, Value.Get<FVector2D>().X, 0);
	AddMovementInput(MoveVector);
}


void APSPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D Direction;
	float Length;

	LookAccumulatedValue += Value.Get<FVector2D>() * LookAccumulatingRate;
	LookAccumulatedValue.ToDirectionAndLength(Direction, Length);
	LookAccumulatedValue = Direction * FMath::Clamp(Length, -1.0f, 1.0f);
}


EPSSuspicionLevel APSPlayerCharacter::GetSuspicionLevel_Implementation()
{
	return EPSSuspicionLevel::High;
}