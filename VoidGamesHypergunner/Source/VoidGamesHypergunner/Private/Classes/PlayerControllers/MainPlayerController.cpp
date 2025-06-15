#include "Classes/PlayerControllers/MainPlayerController.h"

void AMainPlayerController::ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass) {
	if (IsPrimaryPlayer()) {
		Super::ClientSetHUD_Implementation(NewHUDClass);
	}
}
