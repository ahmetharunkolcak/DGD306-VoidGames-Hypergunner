#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "Blueprint/UserWidget.h"

#include "Interfaces/TimerContainable.h"
#include "Interfaces/VisualContainable.h"

#include "MainHUD.generated.h"

UCLASS()
class VOIDGAMESHYPERGUNNER_API AMainHUD : public AHUD {
	GENERATED_BODY()

	public:
		AMainHUD();

	protected:
		virtual void BeginPlay() override;
		virtual void Tick(const float DeltaSeconds) override;

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|Widgets")
		TSubclassOf<UUserWidget> InGameStatusWidget = nullptr;

	private:
		bool bIsTimeObtainableFromGameMode = false;

		UPROPERTY()
		TScriptInterface<ITimerContainable> TimerContainableInterfaceOfGameMode = nullptr;

		UPROPERTY()
		TScriptInterface<IVisualContainable> VisualContainableInterfaceOfInGameStatusWidget = nullptr;
};
