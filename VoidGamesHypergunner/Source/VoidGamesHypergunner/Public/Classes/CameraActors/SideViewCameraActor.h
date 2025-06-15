#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"

#include "Classes/Characters/PlayerCharacter.h"

#include "SideViewCameraActor.generated.h"

class ACharacter;

UCLASS()
class VOIDGAMESHYPERGUNNER_API ASideViewCameraActor : public ACameraActor {
	GENERATED_BODY()

	public:
		ASideViewCameraActor();

		UFUNCTION()
		void HandleOnDeath(const int32 PlayerIndex);

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

		UPROPERTY(EditAnywhere, Category = "Custom|Settings")
		FVector EndGamePositionOffset = FVector::ZeroVector;

		UPROPERTY(EditAnywhere, Category = "Custom|Settings")
		FRotator EndGameRotationOffset = FRotator::ZeroRotator;

		UPROPERTY(EditAnywhere, Category = "Custom|Settings")
		float EndGameRepositionAnimationTime = 1.0f;

	private:
		FVector DefaultWorldLocation = FVector::ZeroVector;

		UPROPERTY()
		ACharacter* Player1CharacterReference = nullptr;

		UPROPERTY()
		ACharacter* Player2CharacterReference = nullptr;

		FVector Player1WorldLocation = FVector::ZeroVector;
		FVector Player2WorldLocation = FVector::ZeroVector;

		bool bShouldRepositionForEnd = false;
		bool bDidRepositioningEnded = false;
		int32 ZoomToPlayerIndex = -1;

		FVector StartEndGameLocation = FVector::ZeroVector;
		FRotator StartEndGameRotation = FRotator::ZeroRotator;
		FVector TargetEndGameLocation = FVector::ZeroVector;
		FRotator TargetEndGameRotation = FRotator::ZeroRotator;
		float EndGameCurrentRepositioningTime = 0.0f;

		bool bIsTickValid = false;
		int32 TickCount = 0;
};
