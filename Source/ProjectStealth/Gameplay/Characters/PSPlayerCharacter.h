// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectStealth/ProjectStealth.h"
#include "ProjectStealth/Gameplay/Characters/PSCharacterBase.h"
#include "PSPlayerCharacter.generated.h"


class UPSInteractionComponent;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;


UCLASS()
class PROJECTSTEALTH_API APSPlayerCharacter : public APSCharacterBase
{
	GENERATED_BODY()

public:

	APSPlayerCharacter();

	virtual EPSSuspicionLevel GetSuspicionLevel_Implementation() override;

protected:

	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Project Stealth|Inputs")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Project Stealth|Inputs")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Project Stealth|Inputs")
	TObjectPtr<UPSInteractionComponent> InteractionComponent;

public:

	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Project Stealth|Inputs")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Project Stealth|Inputs")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Project Stealth|Inputs")
	TObjectPtr<UInputAction> LookInputAction;

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=0.0f), Category = "Project Stealth|Inputs")
	float LookAccumulatingRate;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.0f), Category = "Project Stealth|Inputs")
	float LookSmoothing;

	FVector2D LookAccumulatedValue;


	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};
