#include "Classes/HUDs/MainHUD.h"

#include "Classes/Characters/PlayerCharacter.h"
#include "Classes/UserWidgets/InGameWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/TimerContainable.h"
#include "Interfaces/VisualContainable.h"
#include "Kismet/GameplayStatics.h"

AMainHUD::AMainHUD() {
	PrimaryActorTick.bCanEverTick = true;
}

void AMainHUD::SetInGameCharacterImage(UTexture2D* ImageToSet, const bool bIsForLeftPlayer) {
	this -> CachedCharacterData.Add(bIsForLeftPlayer, ImageToSet);
}

void AMainHUD::TriggerReturnScreen(const bool bIsTimeUp) {
	if (UGameplayStatics::SetGamePaused(GetWorld(), true)) {
		UInGameWidget* InGameWidget = Cast<UInGameWidget>(this -> InGameWidgetInstance);
		InGameWidget -> SetIsFocusable(true);

		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			PlayerController != nullptr) {
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(InGameWidget -> TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController -> SetInputMode(InputMode);
			PlayerController -> bShowMouseCursor = true;
		}

		InGameWidget -> PlayAnimation(InGameWidget -> GetTriggerButtonScreenAnimation());
		if (bIsTimeUp) {
			InGameWidget -> PlayAnimation(InGameWidget -> GetSwitchToFinishScreenAnimation());
		}
	}
}

void AMainHUD::BeginPlay() {
	Super::BeginPlay();

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController != nullptr) {
		FInputModeGameOnly InputMode;
		PlayerController -> SetInputMode(InputMode);
		PlayerController -> bShowMouseCursor = false;
	}

	if (this -> InGameStatusWidget == nullptr) {
		UE_LOG(LogTemp,
		       Warning,
		       TEXT("AMainHUD::BeginPlay: InGameStatusWidget is nullptr!"));

		this -> bIsTimeObtainableFromGameMode = false;
		return;
	}

	UWorld* CurrentWorld = GetWorld();
	if (CurrentWorld == nullptr) {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("AMainHUD::BeginPlay: CurrentWorld is nullptr!"));

		this -> bIsTimeObtainableFromGameMode = false;
		return;
	}

	this -> InGameWidgetInstance = CreateWidget<UUserWidget>(CurrentWorld, this -> InGameStatusWidget);
	TArray<AActor*> SpawnedCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), SpawnedCharacters);
	Cast<UInGameWidget>(this -> InGameWidgetInstance) -> SetupPlayerListeners(SpawnedCharacters);

	if (this -> InGameWidgetInstance == nullptr) {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("AMainHUD::BeginPlay: Failed to create InGameStatusWidget instance with the given class!"));

		this -> bIsTimeObtainableFromGameMode = false;
		return;
	}

	AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(CurrentWorld);
	if (CurrentGameMode == nullptr) {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("AMainHUD::BeginPlay: CurrentGameMode is nullptr!"));

		this -> bIsTimeObtainableFromGameMode = false;
		return;
	}

	if (!CurrentGameMode -> Implements<UTimerContainable>()) {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("AMainHUD::BeginPlay: CurrentGameMode does not implement ITimerContainable interface!"));

		this -> bIsTimeObtainableFromGameMode = false;
		return;
	}

	if (!this -> InGameWidgetInstance -> Implements<UVisualContainable>()) {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("AMainHUD::BeginPlay: InGameStatusWidget does not implement IVisualContainable interface!"));

		this -> bIsTimeObtainableFromGameMode = false;
		return;
	}

	this -> TimerContainableInterfaceOfGameMode = CurrentGameMode;
	const float GameTime = this -> TimerContainableInterfaceOfGameMode -> GetGameplayTime(false);

	this -> VisualContainableInterfaceOfInGameStatusWidget = this -> InGameWidgetInstance;
	this -> VisualContainableInterfaceOfInGameStatusWidget -> SetTimer(GameTime);

	this -> ApplyCharacterImages();

	this -> bIsTimeObtainableFromGameMode = true;

	this -> InGameWidgetInstance -> AddToViewport();
}

void AMainHUD::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (!this -> bIsTimeObtainableFromGameMode) {
		return;
	}

	const float CurrentTime = this -> TimerContainableInterfaceOfGameMode -> GetGameplayTime(true);
	this -> VisualContainableInterfaceOfInGameStatusWidget -> SetTimer(CurrentTime);
}

void AMainHUD::ApplyCharacterImages() {
	for (const TPair<bool, UTexture2D*>& CachedData : this -> CachedCharacterData) {
		if (CachedData.Key) {
			this -> VisualContainableInterfaceOfInGameStatusWidget -> SetLeftImage(CachedData.Value);
		} else {
			this -> VisualContainableInterfaceOfInGameStatusWidget -> SetRightImage(CachedData.Value);
		}
	}
}
