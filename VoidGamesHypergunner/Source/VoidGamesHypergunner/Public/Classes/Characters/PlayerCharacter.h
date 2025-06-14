#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InputActionValue.h"
#include "Structs/InputBindingData.h"

#include "Interfaces/HealthComponentContainable.h"

#include "Structs/AnimationListData.h"

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

		UFUNCTION()
		void DoNormalAttack(const FInputActionValue& Value);

		UFUNCTION()
		void DoHeavyAttack(const FInputActionValue& Value);

		UFUNCTION()
		void DoSkillAttack(const FInputActionValue& Value);

		UFUNCTION()
		void Dodge(const FInputActionValue& Value);

		void GetHit();
		void Victory();
		void Defeat();

		UFUNCTION(BlueprintCallable)
		FAnimationListData FindAnimationsByName(const FName Name) const;

		UFUNCTION(BlueprintCallable)
		bool IsAnyAnimationPlayingInGivenList(const TArray<FAnimationData> AnimationDataArray) const;

		float PlayAnimationOf(const TArray<FAnimationData>& Array, const int32 Index, const int32 AttackType);
		void ResetCombos();

		virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|Input")
		UInputMappingContext* InputMappingContext = nullptr;

		UPROPERTY(EditAnywhere, Category = "Custom|Input")
		TArray<FInputBindingData> InputBindingData = {};

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom|Animation")
		TMap<FName, FAnimationListData> AnimationData = {};

		UPROPERTY(EditAnywhere, Category = "Custom|Movement|BackwardsSpeedMultiplier")
		float BackwardsMovementSpeedMultiplier = 0.6f;

		UPROPERTY(BlueprintReadOnly);
		UHealthComponent* HealthComponent = nullptr;

		UPROPERTY(EditAnywhere, Category = "Custom|Health")
		float MaximumHealth = 100.0f;

	private:
		bool bIsAnimationPlaying = false;
		int32 BasicAttackIndex = 0;
		int32 HeavyAttackIndex = 0;
		int32 SkillAttackIndex = 0;
		FTimerHandle ResetComboTimer = {};
};
