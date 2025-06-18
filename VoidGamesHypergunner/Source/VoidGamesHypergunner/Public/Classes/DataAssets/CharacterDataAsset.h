#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Structs/CharacterSelectionData.h"
#include "CharacterDataAsset.generated.h"

UCLASS()
class VOIDGAMESHYPERGUNNER_API UCharacterDataAsset : public UDataAsset {
	GENERATED_BODY()

	public:
		UPROPERTY(EditAnywhere)
		TArray<FCharacterSelectionData> Characters = {};
};
