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

		UPROPERTY(EditAnywhere, Category = "Custom|Input|Actions", meta = (EditCondition = "bCanEditInputActions", EditConditionHides))
		UInputAction* MoveAction = nullptr;

	private:
	#if WITH_EDITOR
		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

	private:
	#if WITH_EDITORONLY_DATA
		UPROPERTY()
		bool bCanEditInputActions = false;
	#endif
};
