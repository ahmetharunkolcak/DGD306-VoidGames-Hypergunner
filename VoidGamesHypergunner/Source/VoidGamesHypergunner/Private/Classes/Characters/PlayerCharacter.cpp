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
				Subsystem -> AddMappingContext(this->InputMappingContext, 0);
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
		EnhancedInputComponent -> BindAction(this -> MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	}
}
