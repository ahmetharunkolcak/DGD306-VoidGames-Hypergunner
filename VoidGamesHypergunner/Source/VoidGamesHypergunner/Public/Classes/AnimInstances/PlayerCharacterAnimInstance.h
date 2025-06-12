#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "GameFramework/Pawn.h"

#include "PlayerCharacterAnimInstance.generated.h"

UCLASS()
class VOIDGAMESHYPERGUNNER_API UPlayerCharacterAnimInstance : public UAnimInstance {
	GENERATED_BODY()

	protected:
		virtual void NativeBeginPlay() override;
		virtual void NativeUpdateAnimation(const float DeltaSeconds) override;

	protected:
		UPROPERTY(BlueprintReadOnly)
		float MovementSpeed = 0.0f;

	private:
		UPROPERTY()
		TWeakObjectPtr<APawn> CharacterReference = nullptr;
};
