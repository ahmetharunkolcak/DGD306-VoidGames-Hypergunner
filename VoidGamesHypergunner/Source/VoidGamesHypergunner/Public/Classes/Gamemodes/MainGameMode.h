#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Structs/CharacterSelectionData.h"

#include "Interfaces/TimerContainable.h"

#include "MainGameMode.generated.h"

UCLASS()
class VOIDGAMESHYPERGUNNER_API AMainGameMode : public AGameModeBase, public ITimerContainable {
	GENERATED_BODY()

	public:
		AMainGameMode();

		FORCEINLINE virtual float GetGameplayTime(const bool bIsCurrentTime) const override;

		UFUNCTION()
		void HandleDeath(int32 PlayerIndex);

	protected:
		virtual void BeginPlay() override;
		virtual void Tick(const float DeltaSeconds) override;

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|PlayerSelection")
		TArray<FCharacterSelectionData> PlayerCharactersData = {};

		UPROPERTY(EditAnywhere, Category = "Custom|Gameplay")
		float GameTime = 120.0f;

		UPROPERTY(EditAnywhere, Category = "Custom|Music")
		FSoundData BackgroundMusic = {};

	private:
		TArray<int32> SpawnedIndexes = {};

		float CurrentTime = 0.0f;
};
