#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VisualContainable.generated.h"

class UTexture2D;

UINTERFACE(MinimalAPI)
class UVisualContainable : public UInterface {
	GENERATED_BODY()
};

class VOIDGAMESHYPERGUNNER_API IVisualContainable {
	GENERATED_BODY()

	public:
		virtual void SetTimer(const float Time) const = 0;
		virtual void SetLeftImage(UTexture2D* Image) = 0;
		virtual void SetRightImage(UTexture2D* Image) = 0;
		virtual void UpdateHealthFor(const AActor* Player, const bool bIsLeftPlayer) = 0;
};
