#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "SideViewCameraActor.generated.h"

class ACharacter;

UCLASS()
class VOIDGAMESHYPERGUNNER_API ASideViewCameraActor : public ACameraActor {
	GENERATED_BODY()

	public:
		ASideViewCameraActor();

	protected:
		virtual void BeginPlay() override;
		virtual void Tick(const float DeltaTime) override;

		virtual void SetSideViewCamera();

	private:
		void SaveInitialWorldLocation();
		void SavePlayerReferences();

		void SavePlayerCurrentLocations();
		FVector CalculateNewTickLocation(const float DeltaTime) const;

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|Settings")
		float ZoomFactor = 0.25f;

		UPROPERTY(EditAnywhere, Category = "Custom|Settings")
		float CameraSmoothingSpeed = 3.0f;

	private:
		FVector DefaultWorldLocation = FVector::ZeroVector;

		UPROPERTY()
		ACharacter* Player1CharacterReference = nullptr;

		UPROPERTY()
		ACharacter* Player2CharacterReference = nullptr;

		FVector Player1WorldLocation = FVector::ZeroVector;
		FVector Player2WorldLocation = FVector::ZeroVector;
};
