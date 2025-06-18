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

void UMainGameInstance::ResetScores() {
	this -> PlayerScores.Add(0, 0);
	this -> PlayerScores.Add(1, 0);
}

void UMainGameInstance::Init() {
	Super::Init();

	this -> ResetSelections();
	this -> ResetScores();
}

void UMainGameInstance::AddPointFor(const int32 PlayerIndex) {
	const int32* CurrentScoreOfPlayer = this -> PlayerScores.Find(PlayerIndex);
	this -> PlayerScores.Add(PlayerIndex, *CurrentScoreOfPlayer + 1);
}
