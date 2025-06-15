#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "Blueprint/UserWidget.h"

#include "Interfaces/TimerContainable.h"
#include "Interfaces/VisualContainable.h"
#include "Interfaces/WidgetContainable.h"

#include "MainHUD.generated.h"

class UUserWidget;

UCLASS()
class VOIDGAMESHYPERGUNNER_API AMainHUD : public AHUD, public IWidgetContainable {
	GENERATED_BODY()

	public:
		AMainHUD();

		virtual void SetInGameCharacterImage(UTexture2D* ImageToSet, const bool bIsForLeftPlayer) override;
		virtual void TriggerReturnScreen(const bool bIsTimeUp);

	protected:
		virtual void BeginPlay() override;
		virtual void Tick(const float DeltaSeconds) override;

		virtual void ApplyCharacterImages();

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|Widgets")
		TSubclassOf<UUserWidget> InGameStatusWidget = nullptr;

	private:
		bool bIsTimeObtainableFromGameMode = false;

		UPROPERTY()
		TScriptInterface<ITimerContainable> TimerContainableInterfaceOfGameMode = nullptr;

		UPROPERTY()
		TScriptInterface<IVisualContainable> VisualContainableInterfaceOfInGameStatusWidget = nullptr;

		UPROPERTY()
		TMap<bool, UTexture2D*> CachedCharacterData = {};

		UPROPERTY()
		UUserWidget* InGameWidgetInstance = nullptr;
};
