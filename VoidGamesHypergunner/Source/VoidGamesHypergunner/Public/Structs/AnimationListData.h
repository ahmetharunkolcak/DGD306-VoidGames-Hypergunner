#pragma once

#include "CoreMinimal.h"

#include "Structs/AnimationData.h"

#include "AnimationListData.generated.h"

USTRUCT(BlueprintType)
struct FAnimationListData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAnimationData> Montages = {};
};
