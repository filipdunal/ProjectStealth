#pragma once

#include "ProjectStealth/Gameplay/Interfaces/PSTriggerSource.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PSCharacterBase.generated.h"


UCLASS()
class PROJECTSTEALTH_API APSCharacterBase : public ACharacter, public IPSTriggerSource
{
	GENERATED_BODY()

public:

	APSCharacterBase();
};
