#include "Classes/AnimInstances/PlayerCharacterAnimInstance.h"

void UPlayerCharacterAnimInstance::NativeBeginPlay() {
	Super::NativeBeginPlay();

	if (APawn* PlayerPawn = TryGetPawnOwner()) {
		this -> CharacterReference = PlayerPawn;
	}
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(const float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (this -> CharacterReference.IsValid()) {
		const FVector Velocity = this -> CharacterReference.Get() -> GetVelocity();
		const FVector ForwardVector = this -> CharacterReference.Get() -> GetActorForwardVector();
		this -> MovementSpeed = FVector::DotProduct(Velocity, ForwardVector);
	} else {
		if (APawn* PlayerPawn = TryGetPawnOwner()) {
			this -> CharacterReference = PlayerPawn;
		}
	}
}
