#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"

#include "InputBindingData.generated.h"

class UInputAction;
class AActor;

USTRUCT()
struct FInputBindingData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName ActionName = "";

	UPROPERTY(EditAnywhere)
	UInputAction* Action = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<ETriggerEvent> TriggerEvents = {};

	UPROPERTY(EditAnywhere)
	FName FunctionName;
};
