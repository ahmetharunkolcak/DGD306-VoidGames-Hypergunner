#include "VoidGamesHypergunner/Public/Classes/Characters/PlayerCharacter.h"

#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"

APlayerCharacter::APlayerCharacter() {
	PrimaryActorTick.bCanEverTick = false;
}

void APlayerCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	if (APlayerController* PlayerController = Cast<APlayerController>(NewController)) {
		if (ULocalPlayer* LocalPlayer = PlayerController -> GetLocalPlayer()) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer)) {
				Subsystem -> AddMappingContext(this->InputMappingContext, 0);
			}
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value) {
	float MovementScaleValue = Value.Get<float>();

	FRotator Rotation = GetControlRotation();
	FRotator RotationToGetRight = FRotator(Rotation.Roll, 0.0f, Rotation.Yaw);
	FVector WorldActorRightDirection = UKismetMathLibrary::GetRightVector(RotationToGetRight);

	AddMovementInput(WorldActorRightDirection, MovementScaleValue);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent -> BindAction(this -> MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	}
}
