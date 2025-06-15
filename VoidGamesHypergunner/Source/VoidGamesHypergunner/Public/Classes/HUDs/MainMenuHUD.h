#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

UCLASS()
class VOIDGAMESHYPERGUNNER_API AMainMenuHUD : public AHUD {
	GENERATED_BODY()

	public:
		AMainMenuHUD();

	protected:
		virtual void BeginPlay() override;

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|Widgets")
		TSubclassOf<UUserWidget> MainMenuDetailWidget = nullptr;

		UPROPERTY()
		UUserWidget* MainMenuWidgetInstance = nullptr;
};
