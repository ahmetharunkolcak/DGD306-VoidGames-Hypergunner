#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Structs/CharacterSelectionData.h"

#include "MainGameInstance.generated.h"

UCLASS()
class VOIDGAMESHYPERGUNNER_API UMainGameInstance : public UGameInstance {
	GENERATED_BODY()

	public:
		FORCEINLINE TArray<FCharacterSelectionData>& GetSelectedPlayerCharactersData() { return this -> SelectedPlayerCharactersData; }
		virtual int32 SetDataForNext(const FCharacterSelectionData& Data);
		virtual void ResetSelections();

	protected:

	private:
		TArray<FCharacterSelectionData> SelectedPlayerCharactersData = {};
};
