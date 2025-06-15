#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

UCLASS()
class VOIDGAMESHYPERGUNNER_API AMainPlayerController : public APlayerController {
	GENERATED_BODY()

	public:
		virtual void ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass) override;
};
