#include "Classes/CameraActors/SideViewCameraActor.h"

#include "Classes/Characters/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

ASideViewCameraActor::ASideViewCameraActor() {
	PrimaryActorTick.bCanEverTick = true;
}

void ASideViewCameraActor::HandleOnDeath(const int32 PlayerIndex) {
	switch (PlayerIndex) {
		case 0: {
			this -> EndGamePositionOffset.X *= -1;
			this -> EndGameRotationOffset.Yaw *= -1;
			break;
		}

		case 1:
		default: {
			break;
		}
	}

	this -> StartEndGameLocation = GetActorLocation();
	this -> StartEndGameRotation = GetActorRotation();
	this -> TargetEndGameLocation = this -> StartEndGameLocation + this -> EndGamePositionOffset;
	this -> TargetEndGameRotation = this -> StartEndGameRotation + this -> EndGameRotationOffset;
	this -> bShouldRepositionForEnd = true;
	this -> ZoomToPlayerIndex = PlayerIndex;
	this -> EndGameCurrentRepositioningTime = 0.0f;
}

void ASideViewCameraActor::BeginPlay() {
	Super::BeginPlay();

	SetActorTickEnabled(false);

	this -> SaveInitialWorldLocation();

	if (const UWorld* CurrentWorld = GetWorld()) {
		FTimerManager& WorldTimerManager = CurrentWorld -> GetTimerManager();
		WorldTimerManager.SetTimerForNextTick(this, &ASideViewCameraActor::SavePlayerReferences);
		WorldTimerManager.SetTimerForNextTick(this, &ASideViewCameraActor::SetSideViewCamera);
	}
}

void ASideViewCameraActor::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);

	if (this -> bDidRepositioningEnded) {
		SetActorTickEnabled(false);
		return;
	}

	if (this -> bShouldRepositionForEnd) {
		if (FMath::IsNearlyEqual(this -> EndGameCurrentRepositioningTime, this -> EndGameRepositionAnimationTime, 0.001f)) {
			this -> bDidRepositioningEnded = true;
			this -> EndGameCurrentRepositioningTime = this -> EndGameRepositionAnimationTime;
			SetActorLocation(this -> TargetEndGameLocation);
			SetActorRotation(this -> TargetEndGameRotation);
			return;
		}

		this -> EndGameCurrentRepositioningTime += DeltaTime;
		const float RawAlpha = this -> EndGameCurrentRepositioningTime / this -> EndGameRepositionAnimationTime;
		const float Alpha = FMath::Clamp(FMath::InterpEaseInOut(0.0f, 1.0f, RawAlpha, this -> CameraSmoothingSpeed), 0.0f, 1.0f);
		const FVector NewLocation = FMath::Lerp(this -> StartEndGameLocation, this -> TargetEndGameLocation, Alpha);
		const FRotator NewRotation = FMath::Lerp(this -> StartEndGameRotation, this -> TargetEndGameRotation, Alpha);
		SetActorLocation(NewLocation);
		SetActorRotation(NewRotation);


		if (FMath::IsNearlyEqual(Alpha, 1.0f)) {
			this -> bDidRepositioningEnded = true;
			this -> EndGameCurrentRepositioningTime = this -> EndGameRepositionAnimationTime;
			SetActorLocation(this -> TargetEndGameLocation);
			SetActorRotation(this -> TargetEndGameRotation);
		}
		return;
	}

	this -> SavePlayerCurrentLocations();
	const FVector NewLocation = this -> CalculateNewTickLocation(DeltaTime);
	SetActorLocation(NewLocation);
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

void ASideViewCameraActor::SaveInitialWorldLocation() {
	this -> DefaultWorldLocation = GetActorLocation();
}

void ASideViewCameraActor::SavePlayerReferences() {
	if (const UWorld* CurrentWorld = GetWorld()) {
		TArray<AActor*> PlayerCharacters = {};
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), PlayerCharacters);
		bool Player1Exists = false;
		bool Player2Exists = false;
		for (AActor* PlayerActor : PlayerCharacters) {
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerActor);
				PlayerCharacter != nullptr) {
				if (!PlayerCharacter -> OnDeath.IsAlreadyBound(this, &ASideViewCameraActor::HandleOnDeath)) {
					PlayerCharacter -> OnDeath.AddDynamic(this, &ASideViewCameraActor::HandleOnDeath);
				}

				switch (PlayerCharacter -> GetPlayerIndex()) {
					case 0: {
						this -> Player1CharacterReference = PlayerCharacter;
						Player1Exists = true;
						break;
					}

					case 1: {
						this -> Player2CharacterReference = PlayerCharacter;
						Player2Exists = true;
						break;
					}

					default: {
						break;
					}
				}
			}
		}

		const bool ArePlayersValid = Player1Exists && Player2Exists;
		if (!ArePlayersValid) {
			FTimerManager& WorldTimerManager = CurrentWorld -> GetTimerManager();

			WorldTimerManager.SetTimerForNextTick(this, &ASideViewCameraActor::SavePlayerReferences);
			if (!Player1Exists) {
				WorldTimerManager.SetTimerForNextTick(this, &ASideViewCameraActor::SetSideViewCamera);
			}
		}
		SetActorTickEnabled(ArePlayersValid);
	}
}

void ASideViewCameraActor::SavePlayerCurrentLocations() {
	this -> Player1WorldLocation = this -> Player1CharacterReference -> GetActorLocation();
	this -> Player2WorldLocation = this -> Player2CharacterReference -> GetActorLocation();
}

FVector ASideViewCameraActor::CalculateNewTickLocation(const float DeltaTime) const {
	const FVector MiddleLocation = (this -> Player1WorldLocation + this -> Player2WorldLocation) / 2.0f;
	const FVector Difference = UKismetMathLibrary::Vector_GetAbs(this -> Player1WorldLocation - this -> Player2WorldLocation);
	
	const FVector TargetLocation = FVector(
		this -> DefaultWorldLocation.X + MiddleLocation.X,
		this -> DefaultWorldLocation.Y + (Difference.X * ZoomFactor),
		this -> DefaultWorldLocation.Z + MiddleLocation.Z
	);

	if (const FVector CurrentLocation = GetActorLocation();
		CurrentLocation != TargetLocation) {
		return UKismetMathLibrary::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, this -> CameraSmoothingSpeed);
	} else {
		return CurrentLocation;
	}
}
