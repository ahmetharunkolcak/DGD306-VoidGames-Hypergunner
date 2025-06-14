#pragma once

#include "CoreMinimal.h"

#include "AnimationData.generated.h"

USTRUCT(BlueprintType)
struct FAnimationData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PlayRate = 1.0f;
};
