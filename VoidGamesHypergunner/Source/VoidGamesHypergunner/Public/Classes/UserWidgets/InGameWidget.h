#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/VisualContainable.h"
#include "InGameWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;
class UButton;
class APlayerCharacter;

UCLASS()
class VOIDGAMESHYPERGUNNER_API UInGameWidget : public UUserWidget, public IVisualContainable {
	GENERATED_BODY()

	public:
		virtual void SetTimer(const float Time) const override;
		virtual void SetLeftImage(UTexture2D* Image) override;
		virtual void SetRightImage(UTexture2D* Image) override;
		virtual void UpdateHealthFor(const AActor* Player, const bool bIsLeftPlayer) override;

		void SetupPlayerListeners();

		UFUNCTION()
		void HandleHealthChanged(int32 PlayerIndex);

		UFUNCTION()
		void HandleDeath(int32 PlayerIndex);

		UFUNCTION()
		void OnRematchClicked();

		UFUNCTION()
		void OnReturnToMainMenuClicked();

		FORCEINLINE UWidgetAnimation* GetTriggerButtonScreenAnimation() const { return this -> TriggerButtonScreen; }
		FORCEINLINE UWidgetAnimation* GetSwitchToFinishScreenAnimation() const { return this -> SwitchToFinishScreen; }

	protected:
		virtual void NativeConstruct() override;

	protected:
		virtual FText GetFormattedTime(float Time) const;
		virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

		UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBarL = nullptr;

		UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBarR = nullptr;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* HealthValueL = nullptr;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* HealthValueR = nullptr;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* TimeValue = nullptr;

		UPROPERTY(meta = (BindWidget))
		UImage* ImageL = nullptr;

		UPROPERTY(meta = (BindWidget))
		UImage* ImageR = nullptr;

		UPROPERTY(meta = (BindWidget))
		UButton* RematchButton = nullptr;

		UPROPERTY(meta = (BindWidget))
		UButton* ReturnToMainMenuButton = nullptr;

		UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* SwitchToFinishScreen = nullptr;

		UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* TriggerButtonScreen = nullptr;

		UPROPERTY()
		TArray<APlayerCharacter*> TrackedPlayers;

		UPROPERTY(EditAnywhere, Category = "Custom|Animation")
		float HealthBarUpdateAnimationTime = 1.0f;

		UPROPERTY(EditAnywhere, Category = "Custom|Animation")
		float AnimationInterpolationSpeed = 3.0f;

	private:
		bool bIsAnimationInterruptedForL = false;
		bool bIsAnimationInterruptedForR = false;

		float LastCurrentHealthForBarL = 100.0f;
		float LastCurrentHealthForBarR = 100.0f;

		float CachedCurrentHealthForBarL = 100.0f;
		float CachedCurrentHealthForBarR = 100.0f;

		float TargetCurrentHealthForBarL = 100.0f;
		float TargetCurrentHealthForBarR = 100.0f;

		float TargetMaximumHealthForBarL = 100.0f;
		float TargetMaximumHealthForBarR = 100.0f;

		float TargetHealthForBarL = 1.0f;
		float TargetHealthForBarR = 1.0f;

		float AnimTimeForHealthBarL = 0.0f;
		float AnimTimeForHealthBarR = 0.0f;

		bool bIsUpdatingLeftHealthBar = false;
		bool bIsUpdatingRightHealthBar = false;
};
