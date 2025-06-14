#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interfaces/Spawnable.h"

#include "SpawnPoint.generated.h"

UCLASS()
class VOIDGAMESHYPERGUNNER_API ASpawnPoint : public AActor, public ISpawnable {
	GENERATED_BODY()

	public:
		ASpawnPoint();

		FORCEINLINE virtual int32 GetSpawnIndex() const override { return this -> SpawnerIndex; }

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|Spawn");
		int32 SpawnerIndex = 0;
};
