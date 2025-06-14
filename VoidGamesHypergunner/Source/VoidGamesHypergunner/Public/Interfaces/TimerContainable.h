
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TimerContainable.generated.h"

UINTERFACE(MinimalAPI)
class UTimerContainable : public UInterface {
	GENERATED_BODY()
};

class VOIDGAMESHYPERGUNNER_API ITimerContainable {
	GENERATED_BODY()

	public:
		FORCEINLINE virtual float GetGameplayTime(const bool bIsCurrentTime) const = 0;
};
