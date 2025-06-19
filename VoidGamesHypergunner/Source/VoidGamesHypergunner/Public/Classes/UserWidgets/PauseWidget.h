#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

class UButton;

UCLASS()
class VOIDGAMESHYPERGUNNER_API UPauseWidget : public UUserWidget {
	GENERATED_BODY()

	public:
		UFUNCTION()
		virtual void OnContinueButtonClicked();

		UFUNCTION()
		virtual void OnReturnToMainMenuButtonClicked();

		UFUNCTION()
		virtual void OnQuitGameButtonClicked();

		virtual void SetTimerForKeyboardFocus(UWorld* World, const float Delay);

	protected:
		virtual void NativeConstruct() override;
		virtual void NativeDestruct() override;

		virtual void SetKeyboardFocusToContinueButton() const;

	protected:
		UPROPERTY(meta = (BindWidget))
		UButton* ContinueButton = nullptr;

		UPROPERTY(meta = (BindWidget))
		UButton* ReturnToMainMenuButton = nullptr;

		UPROPERTY(meta = (BindWidget))
		UButton* QuitGameButton = nullptr;
};
