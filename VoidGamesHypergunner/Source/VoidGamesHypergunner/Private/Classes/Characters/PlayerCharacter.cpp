#include "VoidGamesHypergunner/Public/Classes/Characters/PlayerCharacter.h"

#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

APlayerCharacter::APlayerCharacter() {
	PrimaryActorTick.bCanEverTick = false;
}

void APlayerCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	if (const APlayerController* PlayerController = Cast<APlayerController>(NewController)) {
		if (const ULocalPlayer* LocalPlayer = PlayerController -> GetLocalPlayer()) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer)) {
				if (this -> InputMappingContext != nullptr) {
					Subsystem -> AddMappingContext(this -> InputMappingContext, 0);
				} else {
					UE_LOG(LogTemp,
					       Warning,
					       TEXT("APlayerCharacter::PossessedBy: InputMappingContext is nullptr!"));
				}
			}
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value) {
	const float MovementScaleValue = Value.Get<float>();
	const FVector MovementDirection = FVector::ForwardVector;
	AddMovementInput(MovementDirection, MovementScaleValue);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (this -> MoveAction != nullptr) {
			EnhancedInputComponent -> BindAction(this -> MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		} else {
			UE_LOG(LogTemp,
			       Warning,
			       TEXT("APlayerCharacter::SetupInputComponent: MoveAction is nullptr!"));
		}
	}
}
