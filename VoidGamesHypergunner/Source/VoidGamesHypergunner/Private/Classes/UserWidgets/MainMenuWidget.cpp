﻿#include "Classes/UserWidgets/MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::OnPlayButtonClicked() {
	this -> OnPlayClicked.Broadcast();
}

void UMainMenuWidget::OnQuitButtonClicked() {
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

void UMainMenuWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (this -> PlayButton != nullptr) {
		this -> PlayButton -> OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayButtonClicked);
	} else {
		UE_LOG(LogTemp,
			Warning,
			TEXT("UMainMenuWidget::NativeConstruct: Tried to add a click event to PlayButton button but it was null!"));
	}

	if (this -> QuitButton != nullptr) {
		this -> QuitButton -> OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitButtonClicked);
	} else {
		UE_LOG(LogTemp,
			Warning,
			TEXT("UMainMenuWidget::NativeConstruct: Tried to add a click event to QuitButton button but it was null!"));
	}
}

void UMainMenuWidget::NativeDestruct() {
	Super::NativeDestruct();

	this -> PlayButton -> OnClicked.RemoveAll(this);
	this -> QuitButton -> OnClicked.RemoveAll(this);
}

void UMainMenuWidget::SetTimerForKeyboardFocus(UWorld* World, const float Delay) {
	FTimerHandle KeyboardFocusTimerHandle;
	World -> GetTimerManager().SetTimer(
		KeyboardFocusTimerHandle,
		this,
		&UMainMenuWidget::SetKeyboardFocusToPlayButton,
		Delay,
		false
	);
}

void UMainMenuWidget::SetKeyboardFocusToPlayButton() const {
	this -> PlayButton -> SetKeyboardFocus();
}
