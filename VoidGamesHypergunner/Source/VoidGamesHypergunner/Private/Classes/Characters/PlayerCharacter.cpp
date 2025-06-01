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

#if WITH_EDITOR

void APlayerCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property
		                                  ? PropertyChangedEvent.Property->GetFName()
		                                  : NAME_None;
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APlayerCharacter, InputMappingContext)) {
		this -> bCanEditInputActions = (this -> InputMappingContext != nullptr);
		if (!this -> bCanEditInputActions) {
			if (const UClass* Class = GetClass()) {
				for (TFieldIterator<FProperty> PropertyIterator(Class); PropertyIterator; ++PropertyIterator) {
					const FProperty* Property = *PropertyIterator;
					if (const FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property)) {
						if (const FString Category = Property -> GetMetaData(TEXT("Category"));
							Category == TEXT("Custom|Input|Actions")) {
							if (ObjectProperty -> PropertyClass -> IsChildOf(UInputAction::StaticClass())) {
								void* PropertyAddress = Property -> ContainerPtrToValuePtr<void>(this);
								ObjectProperty -> SetObjectPropertyValue(PropertyAddress, nullptr);
							}
						}
					}
				}
			}
		}
	}
}

#endif
