#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "SideViewCameraActor.generated.h"

UCLASS()
class VOIDGAMESHYPERGUNNER_API ASideViewCameraActor : public ACameraActor {
	GENERATED_BODY()

	public:
		ASideViewCameraActor();

	protected:
		virtual void BeginPlay() override;

		virtual void SetSideViewCamera();
};
