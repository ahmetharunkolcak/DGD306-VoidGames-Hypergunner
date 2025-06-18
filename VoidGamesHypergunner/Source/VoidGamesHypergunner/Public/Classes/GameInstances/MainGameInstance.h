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

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|CharacterSelection")
		UCharacterDataAsset* CharacterDataAsset = nullptr;

	private:
		TArray<FCharacterSelectionData> SelectedPlayerCharactersData = {};
};
