#include "Classes/CameraActors/SideViewCameraActor.h"

#include "GameFramework/PlayerController.h"

ASideViewCameraActor::ASideViewCameraActor() {
	PrimaryActorTick.bCanEverTick = false;
}

void ASideViewCameraActor::BeginPlay() {
	Super::BeginPlay();

	if (const UWorld* CurrentWorld = GetWorld()) {
		FTimerManager& WorldTimerManager = CurrentWorld -> GetTimerManager();
		WorldTimerManager.SetTimerForNextTick(this, &ASideViewCameraActor::SetSideViewCamera);
	}
}

void ASideViewCameraActor::SetSideViewCamera() {
	if (const UWorld* CurrentWorld = GetWorld()) {
		for (FConstPlayerControllerIterator Iterator = CurrentWorld -> GetPlayerControllerIterator(); Iterator; ++Iterator) {
			if (APlayerController* PlayerController = Iterator -> Get()) {
				PlayerController -> SetViewTarget(this);
			}
		}
	}
}
