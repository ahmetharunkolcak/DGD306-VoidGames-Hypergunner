#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Structs/CharacterSelectionData.h"

#include "MainGameInstance.generated.h"

class UCharacterDataAsset;

UCLASS()
class VOIDGAMESHYPERGUNNER_API UMainGameInstance : public UGameInstance {
	GENERATED_BODY()

	public:
		FORCEINLINE const TArray<FCharacterSelectionData>& GetSelectedPlayerCharactersData() const { return SelectedPlayerCharactersData; }

		virtual int32 SetDataForNext(const FCharacterSelectionData& Data);
		virtual void ResetSelections();
		virtual void ResetScores();
		virtual void Init() override;

		FORCEINLINE virtual TArray<int32> GetCurrentScore() { return TArray({*this -> PlayerScores.Find(0), *this -> PlayerScores.Find(1)}); }
		virtual void AddPointFor(const int32 PlayerIndex);

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|CharacterSelection")
		UCharacterDataAsset* CharacterDataAsset = nullptr;

	private:
		TArray<FCharacterSelectionData> SelectedPlayerCharactersData = {};

		TMap<int32, int32> PlayerScores = {};
};
