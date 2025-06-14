#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Spawnable.generated.h"

UINTERFACE(MinimalAPI)
class USpawnable : public UInterface {
	GENERATED_BODY()
};

class VOIDGAMESHYPERGUNNER_API ISpawnable {
	GENERATED_BODY()

	public:
		FORCEINLINE virtual int32 GetSpawnIndex() const = 0;
};
