#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayClicked);

class UButton;

UCLASS()
class VOIDGAMESHYPERGUNNER_API UMainMenuWidget : public UUserWidget {
	GENERATED_BODY()

	public:
		UFUNCTION()
		void OnPlayButtonClicked();

		UFUNCTION()
		void OnQuitButtonClicked();

	public:
		FOnPlayClicked OnPlayClicked;

	protected:
		virtual void NativeConstruct() override;

		UPROPERTY(meta = (BindWidget))
		UButton* PlayButton = nullptr;

		UPROPERTY(meta = (BindWidget))
		UButton* QuitButton = nullptr;
};
