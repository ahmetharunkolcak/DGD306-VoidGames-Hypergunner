#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InputActionValue.h"
#include "Classes/ActorComponents/HealthComponent.h"
#include "Structs/InputBindingData.h"

#include "Interfaces/HealthComponentContainable.h"

#include "Structs/AnimationListData.h"
#include "Structs/SoundListData.h"

#include "PlayerCharacter.generated.h"

class AController;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
class UHealthComponent;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, int32, PlayerIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, int32, PlayerIndex);

UCLASS()
class VOIDGAMESHYPERGUNNER_API APlayerCharacter : public ACharacter, public IHealthComponentContainable {
	GENERATED_BODY()

	public:
		APlayerCharacter();

		virtual float GetCharacterHealthRate() const override;
		FORCEINLINE virtual float GetCharacterCurrentHealth() const override { return this -> HealthComponent -> GetCurrentHealth(); }
		FORCEINLINE virtual float GetCharacterMaximumHealth() const override { return this -> HealthComponent -> GetMaximumHealth(); }

		void TryDealingDamage(const float Amount, const bool bShouldPlaySound);
		virtual float TakeDamage(const float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
		FORCEINLINE virtual int32 GetPlayerIndex() const { return this -> PlayerIndex; }
		FORCEINLINE virtual void SetPlayerIndex(const int32 NewIndex) { this -> PlayerIndex = NewIndex; }

	protected:
		virtual void BeginPlay() override;
		virtual void PossessedBy(AController* NewController) override;

		virtual void RequestScoreboardUpdate();

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

		UFUNCTION()
		void PauseGame(const FInputActionValue& Value) const;

		void GetHit();
		void Victory();
		void Defeat();

		const FAnimationListData* FindAnimationsByName(const FName Name) const;
		const FSoundListData* FindSoundsByName(const FName Name) const;

		void PlayAnimationOf(const TArray<FAnimationData>& Array, const int32 Index, const int32 AttackType);
		void PlaySoundOf(const TArray<FSoundData>& Array, const int32 Index) const;

		void ResetCombos();
		void ResetAnimationPlayState();
		void Die();

		virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	public:
		UPROPERTY()
		FOnHealthChangedSignature OnHealthChanged = {};

		UPROPERTY()
		FOnDeathSignature OnDeath = {};

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|Input")
		UInputMappingContext* InputMappingContext = nullptr;

		UPROPERTY(EditAnywhere, Category = "Custom|Input")
		TArray<FInputBindingData> InputBindingData = {};

		UPROPERTY(EditAnywhere, Category = "Custom|Animation")
		TMap<FName, FAnimationListData> AnimationData = {};

		UPROPERTY(EditAnywhere, Category = "Custom|Sound")
		TMap<FName, FSoundListData> SoundData = {};

		UPROPERTY(EditAnywhere, Category = "Custom|Movement|BackwardsSpeedMultiplier")
		float BackwardsMovementSpeedMultiplier = 0.6f;

		UPROPERTY(BlueprintReadOnly);
		UHealthComponent* HealthComponent = nullptr;

		UPROPERTY(EditAnywhere, Category = "Custom|Health")
		float MaximumHealth = 100.0f;

		UPROPERTY(EditAnywhere, Category = "Custom|Attack")
		float AttackRange = 100.0f;

		int32 PlayerIndex = 0;

	private:
		bool bIsAnimationPlaying = false;
		bool bDidTheCharacterDie = false;
		bool bShouldBlockInput = false;
		bool bDidFinishHimTriggered = false;

		int32 NormalAttackIndex = 0;
		int32 HeavyAttackIndex = 0;
		int32 SkillAttackIndex = 0;

		float AnimationCutoffRate = 0.75f;
		FTimerHandle AnimationCountdownTimerHandler = {};
		FTimerHandle ResetComboTimerHandler = {};
};
