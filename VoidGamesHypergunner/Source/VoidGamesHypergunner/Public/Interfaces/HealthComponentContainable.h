#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthComponentContainable.generated.h"

UINTERFACE(MinimalAPI)
class UHealthComponentContainable : public UInterface {
	GENERATED_BODY()
};

class VOIDGAMESHYPERGUNNER_API IHealthComponentContainable {
	GENERATED_BODY()

	public:
		virtual float GetCharacterHealthRate() const = 0;
		FORCEINLINE virtual float GetCharacterCurrentHealth() const = 0;
		FORCEINLINE virtual float GetCharacterMaximumHealth() const = 0;
};
