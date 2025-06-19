#include "Classes/UserWidgets/PauseWidget.h"

#include "Classes/GameInstances/MainGameInstance.h"
#include "Classes/Gamemodes/MainGameMode.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseWidget::OnContinueButtonClicked() {
	Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld())) -> TriggerPause();
}

void UPauseWidget::OnReturnToMainMenuButtonClicked() {
	if (UGameInstance* GameInstance = GetGameInstance()) {
		for (int32 i = GameInstance -> GetNumLocalPlayers() - 1; i > 0; --i) {
			ULocalPlayer* LocalPlayer = GameInstance -> GetLocalPlayerByIndex(i);
			GameInstance -> RemoveLocalPlayer(LocalPlayer);
		}

		Cast<UMainGameInstance>(GameInstance) -> ResetSelections();
		Cast<UMainGameInstance>(GameInstance) -> ResetScores();
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"), true);
}

void UPauseWidget::OnQuitGameButtonClicked() {
	if (UGameInstance* GameInstance = GetGameInstance()) {
		for (int32 i = GameInstance -> GetNumLocalPlayers() - 1; i > 0; --i) {
			ULocalPlayer* LocalPlayer = GameInstance -> GetLocalPlayerByIndex(i);
			GameInstance -> RemoveLocalPlayer(LocalPlayer);
		}
	}

	if (const UWorld* World = GetWorld();
		World != nullptr) {
		if (APlayerController* PlayerController	 = World -> GetFirstPlayerController();
			PlayerController != nullptr) {
			UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
		}
	}
}

void UPauseWidget::NativeConstruct() {
	Super::NativeConstruct();

	this -> ContinueButton -> OnClicked.AddDynamic(this, &UPauseWidget::OnContinueButtonClicked);
	this -> ReturnToMainMenuButton -> OnClicked.AddDynamic(this, &UPauseWidget::OnReturnToMainMenuButtonClicked);
	this -> QuitGameButton -> OnClicked.AddDynamic(this, &UPauseWidget::OnQuitGameButtonClicked);
}

void UPauseWidget::NativeDestruct() {
	Super::NativeDestruct();

	this -> ContinueButton -> OnClicked.RemoveAll(this);
	this -> ReturnToMainMenuButton -> OnClicked.RemoveAll(this);
	this -> QuitGameButton -> OnClicked.RemoveAll(this);
}

void UPauseWidget::SetTimerForKeyboardFocus(UWorld* World, const float Delay) {
	FTimerHandle KeyboardFocusTimerHandle;
	World -> GetTimerManager().SetTimer(
		KeyboardFocusTimerHandle,
		this,
		&UPauseWidget::SetKeyboardFocusToContinueButton,
		Delay,
		false
	);
}

void UPauseWidget::SetKeyboardFocusToContinueButton() const {
	this -> ContinueButton -> SetKeyboardFocus();
}
