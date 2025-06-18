#pragma once

#include "CoreMinimal.h"

#include "Classes/Characters/PlayerCharacter.h"

#include "CharacterSelectionData.generated.h"

class UTexture2D;

USTRUCT()
struct FCharacterSelectionData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText CharacterName = FText::FromString("");

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerCharacter> Character3D = nullptr;

	UPROPERTY(EditAnywhere)
	UTexture2D* Character2D = nullptr;
};
