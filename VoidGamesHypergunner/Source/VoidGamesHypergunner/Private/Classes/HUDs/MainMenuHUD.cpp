#include "Classes/HUDs/MainMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "Classes/UserWidgets/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"


AMainMenuHUD::AMainMenuHUD() {
	PrimaryActorTick.bCanEverTick = false;
}

void AMainMenuHUD::BeginPlay() {
	Super::BeginPlay();

	if (this -> MainMenuDetailWidget == nullptr) {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("AMainMenuHUD::BeginPlay: MainMenuWidget is nullptr!"));

		return;
	}

	UWorld* CurrentWorld = GetWorld();
	if (CurrentWorld == nullptr) {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("AMainMenuHUD::BeginPlay: CurrentWorld is nullptr!"));
		return;
	}

	this -> MainMenuWidgetInstance = CreateWidget<UUserWidget>(CurrentWorld, this -> MainMenuDetailWidget);
	if (this -> MainMenuWidgetInstance == nullptr) {
		UE_LOG(LogTemp,
		       Warning,
		       TEXT("AMainMenuHUD::BeginPlay: Failed to create MainMenuWidget instance with the given class!"));

		return;
	}

	this -> MainMenuWidgetInstance -> AddToViewport();

	UMainMenuWidget* MainMenuWidget = Cast<UMainMenuWidget>(this -> MainMenuWidgetInstance);
	MainMenuWidget -> SetIsFocusable(true);

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController != nullptr) {
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(MainMenuWidget -> TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController -> SetInputMode(InputMode);
		PlayerController -> bShowMouseCursor = true;
	}
}
