#include "VoidGamesHypergunner/Public/Classes/Characters/PlayerCharacter.h"

#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Classes/ActorComponents/HealthComponent.h"

APlayerCharacter::APlayerCharacter() {
	PrimaryActorTick.bCanEverTick = false;

	this -> HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("Health Component"));
}

void APlayerCharacter::BeginPlay() {
	Super::BeginPlay();

	if (this -> HealthComponent != nullptr) {
		this -> HealthComponent -> Initialize(this -> MaximumHealth);
	} else {
		UE_LOG(LogTemp,
		       Warning,
		       TEXT("APlayerCharacter::BeginPlay: HealthComponent is nullptr!"));
	}
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
	if (FMath::IsNearlyZero(MovementScaleValue)) {
		return;
	}

	const FVector ActorForwardDirection = GetActorForwardVector();
	const float FacingDirection = ActorForwardDirection.X >= 0 ? 1.0f : -1.0f;
	float FacedForwardInput = MovementScaleValue * FacingDirection;

	if (const FVector MovementDirection = ActorForwardDirection * FacedForwardInput;
		FVector::DotProduct(MovementDirection, ActorForwardDirection) < 0.0f) {
		FacedForwardInput *= this -> BackwardsMovementSpeedMultiplier;
	}

	AddMovementInput(ActorForwardDirection, FacedForwardInput);
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
