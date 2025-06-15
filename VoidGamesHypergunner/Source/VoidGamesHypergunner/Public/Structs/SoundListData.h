#pragma once

#include "CoreMinimal.h"

#include "SoundData.h"

#include "SoundListData.generated.h"

USTRUCT(BlueprintType)
struct FSoundListData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSoundData> Sounds = {};
};
