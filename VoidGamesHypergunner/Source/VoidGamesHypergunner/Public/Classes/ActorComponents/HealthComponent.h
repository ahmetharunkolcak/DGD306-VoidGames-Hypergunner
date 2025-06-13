#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DisplayName="HealthComponent")
class VOIDGAMESHYPERGUNNER_API UHealthComponent : public UActorComponent {
	GENERATED_BODY()

	public:
		UHealthComponent();

	public:
		virtual void Initialize(const float Health);
		virtual void UpdateHealth(const float Amount, const bool bIsCurrentHealth, const bool bIsPositive);

		UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetCurrentHealth() const { return this -> CurrentHealth; }

		UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetMaximumHealth() const { return this -> MaximumHealth; }

	protected:
		virtual void UpdateCurrentHealth(const float& Amount, const bool& bIsPositive);
		virtual void UpdateMaximumHealth(const float& Amount, const bool& bIsPositive);

	private:
		float CurrentHealth = 0.0f;
		float MaximumHealth = 0.0f;
};
