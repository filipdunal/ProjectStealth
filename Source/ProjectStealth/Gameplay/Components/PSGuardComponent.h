// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectStealth/ProjectStealth.h"
#include "PSGuardComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGuard, Display, All)

class UPSTriggerComponent;

USTRUCT(BlueprintType)
struct FPSSuspicionLevelSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Stealth")
	float AttentionRiseRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Stealth")
	float AttentionCap;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPSAttentionChanged, float, Attention);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPSGuardStateChanged, EPSGuardState, GuardState);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSTEALTH_API UPSGuardComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPSGuardComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBegin(TScriptInterface<IPSTriggerSource> TriggerSource, UPSTriggerComponent* TriggerComponent);

	UFUNCTION()
	void UpdateAttention();


	FTimerHandle UpdateAttentionHandle;

	EPSGuardState GuardState;
	float AttentionLevel;

	UPROPERTY()
	TArray<TObjectPtr<UPSTriggerComponent>> Triggers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0), Category = "Project Stealth")
	float AttentionIncreaseRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0), Category = "Project Stealth")
	float AttentionDecreaseRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Project Stealth")
	TMap<EPSSuspicionLevel, FPSSuspicionLevelSettings> SuspicionLevelSettings;

	const float UpdateAttentionRate = 0.25f;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Project Stealth")
	FORCEINLINE EPSGuardState GetGuardState() const { return GuardState; }

	UFUNCTION(BlueprintCallable, Category = "Project Stealth")
	const float& GetAttentionLevel() const { return AttentionLevel; }


	UPROPERTY(BlueprintAssignable, Category = "Project Stealth")
	FPSAttentionChanged OnAttentionChanged;

	UPROPERTY(BlueprintAssignable, Category = "Project Stealth")
	FPSGuardStateChanged OnGuardStateChanged;
};
