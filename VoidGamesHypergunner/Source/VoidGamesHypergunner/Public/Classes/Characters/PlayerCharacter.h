#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InputActionValue.h"

#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class VOIDGAMESHYPERGUNNER_API APlayerCharacter : public ACharacter {
	GENERATED_BODY()

	public:
		APlayerCharacter();

	protected:
		virtual void PossessedBy(AController* NewController) override;

		void Move(const FInputActionValue& Value);

		virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|Input")
		UInputMappingContext* InputMappingContext = nullptr;

		UPROPERTY(EditAnywhere, Category = "Custom|Input|Actions")
		UInputAction* MoveAction = nullptr;
};
