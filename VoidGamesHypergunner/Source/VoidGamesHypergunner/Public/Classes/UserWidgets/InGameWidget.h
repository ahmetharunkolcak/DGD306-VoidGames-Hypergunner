#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/VisualContainable.h"
#include "InGameWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;
class APlayerCharacter;

UCLASS()
class VOIDGAMESHYPERGUNNER_API UInGameWidget : public UUserWidget, public IVisualContainable {
	GENERATED_BODY()

	public:
		virtual void SetTimer(const float Time) const override;
		virtual void SetLeftImage(UTexture2D* Image) override;
		virtual void SetRightImage(UTexture2D* Image) override;
		virtual void UpdateHealthFor(const AActor* Player, const bool bIsLeftPlayer) override;

		void SetupPlayerListeners(const TArray<AActor*>& Players);

		UFUNCTION()
		void HandleHealthChanged(int32 PlayerIndex);

	protected:
		virtual FText GetFormattedTime(float Time) const;
		virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

		UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBarL = nullptr;

		UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBarR = nullptr;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* TimeValue = nullptr;

		UPROPERTY(meta = (BindWidget))
		UImage* ImageL = nullptr;

		UPROPERTY(meta = (BindWidget))
		UImage* ImageR = nullptr;

		UPROPERTY()
		TArray<APlayerCharacter*> TrackedPlayers;

		UPROPERTY(EditAnywhere, Category = "Custom|Animation")
		float HealthBarUpdateAnimationTime = 1.0f;

		UPROPERTY(EditAnywhere, Category = "Custom|Animation")
		float AnimationInterpolationSpeed = 3.0f;

	private:
		float TargetHealthForBarL = 1.0f;
		float TargetHealthForBarR = 1.0f;

		float AnimTimeForHealthBarL = 0.0f;
		float AnimTimeForHealthBarR = 0.0f;

		bool bIsUpdatingLeftHealthBar = false;
		bool bIsUpdatingRightHealthBar = false;
};
