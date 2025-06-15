#pragma once

#include "CoreMinimal.h"

#include "SoundData.generated.h"

USTRUCT(BlueprintType)
struct FSoundData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundWave* Sound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinPitch = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxPitch = 1.0f;
};
