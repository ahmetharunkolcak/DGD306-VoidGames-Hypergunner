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

float APlayerCharacter::GetCharacterHealthRate() const {
	const float CurHealth = this -> HealthComponent -> GetCurrentHealth();
	const float MaxHealth = this -> HealthComponent -> GetMaximumHealth();
	const float HealthRate = CurHealth / MaxHealth;
	const float ClampedHealthRate = FMath::Clamp(HealthRate, 0.0f, 1.0f);
	return ClampedHealthRate;
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
		for (int32 CurrentIndex = 0; CurrentIndex < this -> InputBindingDatas.Num(); ++CurrentIndex) {
			const auto& [ActionName, Action, TriggerEvents, FunctionName] = this -> InputBindingDatas[CurrentIndex];
			bool bIsFailed = false;

			if (ActionName.IsNone() || ActionName.IsEqual("")) {
				UE_LOG(LogTemp,
				       Warning,
				       TEXT("APlayerCharacter::SetupInputComponent: ActionName is empty!"));
				bIsFailed = true;
			}

			if (Action == nullptr) {
				UE_LOG(LogTemp,
				       Warning,
				       TEXT("APlayerCharacter::SetupInputComponent: Action of %s is nullptr."),
				       *ActionName.ToString());
				bIsFailed = true;
			}

			if (TriggerEvents.Num() == 0) {
				UE_LOG(LogTemp,
				       Warning,
				       TEXT("APlayerCharacter::SetupInputComponent: TriggerEvent of %s is empty!"),
				       *ActionName.ToString());
				bIsFailed = true;
			}

			if (FunctionName.IsNone() || this -> FindFunction(FunctionName) == nullptr) {
				UE_LOG(LogTemp,
					   Warning,
					   TEXT("APlayerCharacter::SetupInputComponent: Function %s of %s is not found!"),
					   *FunctionName.ToString(),
					   *ActionName.ToString());
				bIsFailed = true;
			}

			if (bIsFailed) {
				UE_LOG(LogTemp,
					Warning,
					TEXT("Skipping input action at index %d!"),
					CurrentIndex)
				continue;
			}

			for (const ETriggerEvent& TriggerEvent : TriggerEvents) {
				EnhancedInputComponent -> BindAction(Action, TriggerEvent, this, FunctionName);
			}
		}
	}
}
