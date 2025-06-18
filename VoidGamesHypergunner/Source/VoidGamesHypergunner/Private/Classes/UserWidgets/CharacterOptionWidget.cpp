#include "Classes/UserWidgets/CharacterOptionWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"

#include "Kismet/GameplayStatics.h"
#include "Classes/GameInstances/MainGameInstance.h"
#include "Components/TextBlock.h"

void UCharacterOptionWidget::ResetSelections() {
	if (this -> IndexText1 -> GetVisibility() == ESlateVisibility::Visible) {
		this -> IndexText1 -> SetVisibility(ESlateVisibility::Collapsed);
	}

	if (this -> IndexText2 -> GetVisibility() == ESlateVisibility::Visible) {
		this -> IndexText2 -> SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCharacterOptionWidget::NativePreConstruct() {
	Super::NativePreConstruct();

	this -> CharacterImage -> SetBrushFromTexture(this -> CharacterSelectionData.Character2D);
	this -> NameText -> SetText(this -> CharacterSelectionData.CharacterName);
}

void UCharacterOptionWidget::NativeConstruct() {
	Super::NativeConstruct();

	this -> SelectButton -> OnClicked.AddDynamic(this, &UCharacterOptionWidget::OnButtonClicked);
	this -> ResetSelections();
}


void UCharacterOptionWidget::OnButtonClicked() {
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (const int32 SetIndex = GameInstance -> SetDataForNext(this -> CharacterSelectionData);
		SetIndex == 0) {
		this -> IndexText1 -> SetVisibility(ESlateVisibility::Visible);
	} else if (SetIndex == 1) {
		this -> IndexText2 -> SetVisibility(ESlateVisibility::Visible);
	}
}


