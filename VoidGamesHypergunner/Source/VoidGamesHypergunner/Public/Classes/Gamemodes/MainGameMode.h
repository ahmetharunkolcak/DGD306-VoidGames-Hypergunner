#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Classes/Characters/PlayerCharacter.h"

#include "MainGameMode.generated.h"

UCLASS()
class VOIDGAMESHYPERGUNNER_API AMainGameMode : public AGameModeBase {
	GENERATED_BODY()

	protected:
		virtual void BeginPlay() override;

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|PlayerSelection")
		TArray<TSubclassOf<APlayerCharacter>> PlayerCharacters = {};
};
