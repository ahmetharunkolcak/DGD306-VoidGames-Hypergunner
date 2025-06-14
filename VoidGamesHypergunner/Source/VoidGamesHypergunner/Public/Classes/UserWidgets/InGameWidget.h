#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/VisualContainable.h"
#include "InGameWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class VOIDGAMESHYPERGUNNER_API UInGameWidget : public UUserWidget, public IVisualContainable {
	GENERATED_BODY()

	public:
		virtual void SetTimer(const float Time) const override;
		virtual void UpdateHealthFor(const AActor* Player, const bool bIsLeftPlayer) override;

	protected:
		virtual FText GetFormattedTime(float Time) const;

	protected:
		UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBarL = nullptr;

		UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBarR = nullptr;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* TimeValue = nullptr;
};
