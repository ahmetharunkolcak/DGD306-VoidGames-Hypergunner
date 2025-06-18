#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackClicked);

class UUniformGridPanel;
class UTextBlock;
class UButton;

UCLASS()
class VOIDGAMESHYPERGUNNER_API UCharacterSelectionWidget : public UUserWidget {
	GENERATED_BODY()

	public:
		UFUNCTION()
		virtual void OnResetButtonClicked();

		UFUNCTION()
		virtual void OnPlayButtonClicked();

		UFUNCTION()
		virtual void OnReturnToMainMenuButtonClicked();

		FOnBackClicked OnBackClicked;

	protected:
		virtual void NativeConstruct() override;
		virtual void NativeDestruct() override;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* WarningText = nullptr;

		UPROPERTY(meta = (BindWidget))
		UButton* ResetButton = nullptr;

		UPROPERTY(meta = (BindWidget))
		UButton* PlayButton = nullptr;

		UPROPERTY(meta = (BindWidget))
		UButton* ReturnToMainMenuButton = nullptr;

		UPROPERTY(meta = (BindWidget))
		UUniformGridPanel* CharacterOptionContainerGridPanel = nullptr;
};
