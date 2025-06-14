#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InputActionValue.h"
#include "Structs/InputBindingData.h"

#include "Interfaces/HealthComponentContainable.h"

#include "PlayerCharacter.generated.h"

class AController;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
class UHealthComponent;

UCLASS()
class VOIDGAMESHYPERGUNNER_API APlayerCharacter : public ACharacter, public IHealthComponentContainable {
	GENERATED_BODY()

	public:
		APlayerCharacter();

		virtual float GetCharacterHealthRate() const override;

	protected:
		virtual void BeginPlay() override;
		virtual void PossessedBy(AController* NewController) override;

		UFUNCTION()
		void Move(const FInputActionValue& Value);

		virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|Input")
		UInputMappingContext* InputMappingContext = nullptr;

		UPROPERTY(EditAnywhere, Category = "Custom|Input")
		TArray<FInputBindingData> InputBindingDatas;

		UPROPERTY(EditAnywhere, Category = "Custom|Movement|BackwardsSpeedMultiplier")
		float BackwardsMovementSpeedMultiplier = 0.6f;

		UPROPERTY(BlueprintReadOnly);
		UHealthComponent* HealthComponent = nullptr;

		UPROPERTY(EditAnywhere, Category = "Custom|Health")
		float MaximumHealth = 100.0f;
};
