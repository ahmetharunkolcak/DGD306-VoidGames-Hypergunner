#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/CharacterSelectionData.h"
#include "CharacterOptionWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;

UCLASS()
class VOIDGAMESHYPERGUNNER_API UCharacterOptionWidget : public UUserWidget {
	GENERATED_BODY()

	public:
		virtual void ResetSelections();

	protected:
		virtual void NativePreConstruct() override;
		virtual void NativeConstruct() override;

		UFUNCTION()
		virtual void OnButtonClicked();
		virtual void NativeDestruct() override;

		UPROPERTY(EditAnywhere, Category = "Custom|CharacterData")
		FCharacterSelectionData CharacterSelectionData = {};

		UPROPERTY(meta = (BindWidget))
		UButton* SelectButton = nullptr;

		UPROPERTY(meta = (BindWidget))
		UImage* CharacterImage = nullptr;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* IndexText1 = nullptr;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* IndexText2 = nullptr;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* NameText = nullptr;
};
