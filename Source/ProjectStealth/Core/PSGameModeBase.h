// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PSGameModeBase.generated.h"


class APSGameStateBase;

UCLASS()
class PROJECTSTEALTH_API APSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Project Stealth")
	void EndGame(bool bWin);

	UFUNCTION(BlueprintCallable, Category = "Project Stealth")
	FORCEINLINE bool IsGameEnded() const { return bIsGameEnded; }

protected:

	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadOnly, Category = "Project Stealth")
	TObjectPtr<APSGameStateBase> PSGameState;

	UPROPERTY(BlueprintReadOnly, Category = "Project Stealth")
	bool bIsGameEnded;
};
