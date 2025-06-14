#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Classes/Characters/PlayerCharacter.h"

#include "Interfaces/TimerContainable.h"

#include "MainGameMode.generated.h"

UCLASS()
class VOIDGAMESHYPERGUNNER_API AMainGameMode : public AGameModeBase, public ITimerContainable {
	GENERATED_BODY()

	public:
		AMainGameMode();

		FORCEINLINE virtual float GetGameplayTime(const bool bIsCurrentTime) const override;

	protected:
		virtual void BeginPlay() override;
		virtual void Tick(const float DeltaSeconds) override;

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|PlayerSelection")
		TArray<TSubclassOf<APlayerCharacter>> PlayerCharacters = {};

		UPROPERTY(EditAnywhere, Category = "Custom|Gameplay")
		float GameTime = 120.0f;

	private:
		TArray<int32> SpawnedIndexes = {};

		float CurrentTime = 0.0f;
};
