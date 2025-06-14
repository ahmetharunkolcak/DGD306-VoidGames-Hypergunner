#include "Classes/HUDs/MainHUD.h"

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

void AMainHUD::BeginPlay() {
	Super::BeginPlay();

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

	UUserWidget* InGameStatusWidgetInstance = CreateWidget<UUserWidget>(CurrentWorld, this -> InGameStatusWidget);
	if (InGameStatusWidgetInstance == nullptr) {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("AMainHUD::BeginPlay: Failed to created InGameStatusWidget instance with the given class!"));

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

	if (!InGameStatusWidgetInstance -> Implements<UVisualContainable>()) {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("AMainHUD::BeginPlay: InGameStatusWidget does not implement IVisualContainable interface!"));

		this -> bIsTimeObtainableFromGameMode = false;
		return;
	}

	this -> TimerContainableInterfaceOfGameMode = CurrentGameMode;
	const float GameTime = this -> TimerContainableInterfaceOfGameMode -> GetGameplayTime(false);

	this -> VisualContainableInterfaceOfInGameStatusWidget = InGameStatusWidgetInstance;
	this -> VisualContainableInterfaceOfInGameStatusWidget -> SetTimer(GameTime);

	this -> ApplyCharacterImages();

	this -> bIsTimeObtainableFromGameMode = true;

	InGameStatusWidgetInstance -> AddToViewport();
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
