#include "Classes/AnimNotifies/FinishScreenAnimNotify.h"

#include "Classes/HUDs/MainHUD.h"
#include "Kismet/GameplayStatics.h"

void UFinishScreenAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr) {
		UE_LOG(LogTemp,
		       Warning,
		       TEXT("UFinishScreenAnimNotify::Notify: MeshComp is nullptr!"));

		return;
	}

	UWorld* CurrentWorld = MeshComp -> GetWorld();
	if (CurrentWorld == nullptr) {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("UFinishScreenAnimNotify::Notify: CurrentWorld is nullptr!"));

		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(CurrentWorld, 0);
	if (PlayerController == nullptr) {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("UFinishScreenAnimNotify::Notify: PlayerController is nullptr!"));

		return;
	}

	AMainHUD* HUD = Cast<AMainHUD>(PlayerController -> GetHUD());
	if (HUD == nullptr) {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("UFinishScreenAnimNotify::Notify: HUD is nullptr!"));

		return;
	}

	HUD -> TriggerReturnScreen(false);
}
