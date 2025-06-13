#include "Classes/ActorComponents/HealthComponent.h"

UHealthComponent::UHealthComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::Initialize(const float Health) {
	this -> MaximumHealth = Health;
	this -> CurrentHealth = this -> MaximumHealth;
}

void UHealthComponent::UpdateHealth(const float Amount, const bool bIsCurrentHealth, const bool bIsPositive) {
	if (bIsCurrentHealth) {
		this -> UpdateCurrentHealth(Amount, bIsPositive);
	} else {
		this -> UpdateMaximumHealth(Amount, bIsPositive);
	}
}

void UHealthComponent::UpdateCurrentHealth(const float& Amount, const bool& bIsPositive) {
	const float NewCurrentHealth = this -> CurrentHealth + (bIsPositive ? Amount : -Amount);
	this -> CurrentHealth = FMath::Clamp(NewCurrentHealth, 0.0f, this -> MaximumHealth);
}
void UHealthComponent::UpdateMaximumHealth(const float& Amount, const bool& bIsPositive) {
	const float NewMaximumHealth = this -> MaximumHealth + (bIsPositive ? Amount : -Amount);
	this -> MaximumHealth = FMath::Max(1.0f, NewMaximumHealth);
	this -> CurrentHealth = FMath::Clamp(this -> CurrentHealth, 0.0f, this -> MaximumHealth);
}
