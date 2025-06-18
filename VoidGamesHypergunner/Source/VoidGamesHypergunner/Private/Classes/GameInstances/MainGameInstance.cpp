#include "Classes/GameInstances/MainGameInstance.h"

int32 UMainGameInstance::SetDataForNext(const FCharacterSelectionData& Data) {
	const int32 ArrayLength = this -> SelectedPlayerCharactersData.Num();
	if (ArrayLength == 0) {
		this -> SelectedPlayerCharactersData.Add(Data);
	} else if (ArrayLength == 1) {
		this -> SelectedPlayerCharactersData.Add(Data);
	}

	return ArrayLength;
}

void UMainGameInstance::ResetSelections() {
	this -> SelectedPlayerCharactersData.Empty();
}
