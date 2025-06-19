#include "Classes/UserWidgets/CharacterSelectionWidget.h"

#include "Classes/GameInstances/MainGameInstance.h"
#include "Classes/UserWidgets/CharacterOptionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"

class UCharacterOptionWidget;

void UCharacterSelectionWidget::OnResetButtonClicked() {
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance -> ResetSelections();

	for (UWidget* CharacterOptionWidgetInstance : this -> CharacterOptionContainerGridPanel -> GetAllChildren()) {
		Cast<UCharacterOptionWidget>(CharacterOptionWidgetInstance) -> ResetSelections();
	}

	if (!this -> WarningText -> GetText().ToString().Equals(FString(""))) {
		this -> WarningText -> SetText(FText::FromString(""));
	}
}

void UCharacterSelectionWidget::OnPlayButtonClicked() {
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance -> GetSelectedPlayerCharactersData().Num() != 2) {
		this -> WarningText -> SetText(FText::FromString("At least one player did not select their characters!"));
		return;
	}

	for (int32 i = GameInstance -> GetNumLocalPlayers() - 1; i > 0; --i) {
		ULocalPlayer* LocalPlayer = GameInstance -> GetLocalPlayerByIndex(i);
		GameInstance -> RemoveLocalPlayer(LocalPlayer);
	}

	UGameplayStatics::OpenLevel(GetWorld(), "MainLevel", true);
}

void UCharacterSelectionWidget::OnReturnToMainMenuButtonClicked() {
	this -> OnBackClicked.Broadcast();
}

void UCharacterSelectionWidget::NativeConstruct() {
	Super::NativeConstruct();

	this -> ResetButton -> OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnResetButtonClicked);
	this -> PlayButton -> OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnPlayButtonClicked);
	this -> ReturnToMainMenuButton -> OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnReturnToMainMenuButtonClicked);
}

void UCharacterSelectionWidget::NativeDestruct() {
	Super::NativeDestruct();

	this -> ResetButton -> OnClicked.RemoveAll(this);
	this -> PlayButton -> OnClicked.RemoveAll(this);
	this -> ReturnToMainMenuButton -> OnClicked.RemoveAll(this);
}

void UCharacterSelectionWidget::SetTimerForKeyboardFocus(UWorld* World, const float Delay) {
	FTimerHandle KeyboardFocusTimerHandle;

	World -> GetTimerManager().SetTimer(
		KeyboardFocusTimerHandle,
		this,
		&UCharacterSelectionWidget::SetKeyboardFocusToResetButton,
		Delay,
		false
	);
}

void UCharacterSelectionWidget::SetKeyboardFocusToResetButton() const {
	this -> ResetButton -> SetKeyboardFocus();
}
