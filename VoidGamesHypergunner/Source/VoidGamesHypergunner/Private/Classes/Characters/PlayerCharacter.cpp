#include "VoidGamesHypergunner/Public/Classes/Characters/PlayerCharacter.h"

#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Classes/ActorComponents/HealthComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

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

void APlayerCharacter::TryDealingDamage(const float Amount) {
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * this -> AttackRange;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	if (FHitResult HitResult;
		GetWorld() -> LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, CollisionQueryParams)) {
		if (AActor* HitActor = HitResult.GetActor()) {
			UGameplayStatics::ApplyDamage(HitActor, Amount, this -> GetController(), this, UDamageType::StaticClass());
		}
	}
}

float APlayerCharacter::TakeDamage(const float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (this -> HealthComponent != nullptr) {
		this -> HealthComponent -> UpdateHealth(ActualDamage, true, false);
		this -> OnHealthChanged.Broadcast(this -> PlayerIndex);
		if (this -> HealthComponent -> GetCurrentHealth() <= 0.0f) {
			this -> Die();
		}
	}

	return ActualDamage;
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

void APlayerCharacter::DoNormalAttack(const FInputActionValue& Value) {
	if (!this -> bIsAnimationPlaying) {
		const auto [Montages] = this -> FindAnimationsByName(FName("BasicAttack"));
		const float AnimLength = this -> PlayAnimationOf(Montages, this -> BasicAttackIndex, 0);
		GetWorld() -> GetTimerManager().SetTimer(
			this -> ResetComboTimer,
			this,
			&APlayerCharacter::ResetCombos,
			AnimLength,
			false
		);
	}
}

void APlayerCharacter::DoHeavyAttack(const FInputActionValue& Value) {
	if (!this -> bIsAnimationPlaying) {
		const auto [Montages] = this -> FindAnimationsByName(FName("HeavyAttack"));
		const float AnimLength = this -> PlayAnimationOf(Montages, this -> HeavyAttackIndex, 1);
		GetWorld() -> GetTimerManager().SetTimer(
			this -> ResetComboTimer,
			this,
			&APlayerCharacter::ResetCombos,
			AnimLength,
			false
		);
	}
}

void APlayerCharacter::DoSkillAttack(const FInputActionValue& Value) {
	if (!this -> bIsAnimationPlaying) {
		const auto [Montages] = this -> FindAnimationsByName(FName("SkillAttack"));
		const float AnimLength = this -> PlayAnimationOf(Montages, this -> SkillAttackIndex, 2);
		GetWorld() -> GetTimerManager().SetTimer(
			this -> ResetComboTimer,
			this,
			&APlayerCharacter::ResetCombos,
			AnimLength,
			false
		);
	}
}

void APlayerCharacter::Dodge(const FInputActionValue& Value) {
	if (!this -> bIsAnimationPlaying) {
		const auto [Montages] = this -> FindAnimationsByName(FName("Dodge"));
		const float AnimLength = this -> PlayAnimationOf(Montages, -1, -1);
		GetWorld() -> GetTimerManager().SetTimer(
			this -> ResetComboTimer,
			this,
			&APlayerCharacter::ResetCombos,
			AnimLength,
			false
		);
	}
}
void APlayerCharacter::GetHit() {
	if (!this -> bIsAnimationPlaying) {
		const auto [Montages] = this -> FindAnimationsByName(FName("GetHit"));
		const float AnimLength = this -> PlayAnimationOf(Montages, -1, -1);
		GetWorld() -> GetTimerManager().SetTimer(
			this -> ResetComboTimer,
			this,
			&APlayerCharacter::ResetCombos,
			AnimLength,
			false
		);
	}
}
void APlayerCharacter::Victory() {
	if (!this -> bIsAnimationPlaying) {
		const auto [Montages] = this -> FindAnimationsByName(FName("Victory"));
		const float AnimLength = this -> PlayAnimationOf(Montages, -1, -1);
		GetWorld() -> GetTimerManager().SetTimer(
			this -> ResetComboTimer,
			this,
			&APlayerCharacter::ResetCombos,
			AnimLength,
			false
		);
	}
}

void APlayerCharacter::Defeat() {
	if (!this -> bIsAnimationPlaying) {
		const auto [Montages] = this -> FindAnimationsByName(FName("Defeat"));
		const float AnimLength = this -> PlayAnimationOf(Montages, -1, -1);
		GetWorld() -> GetTimerManager().SetTimer(
			this -> ResetComboTimer,
			this,
			&APlayerCharacter::ResetCombos,
			AnimLength,
			false
		);
	}
}

FAnimationListData APlayerCharacter::FindAnimationsByName(const FName Name) const {
	if (const FAnimationListData* FoundAnimationData = this -> AnimationData.Find(Name);
		FoundAnimationData != nullptr) {
		return *FoundAnimationData;
	}

	return FAnimationListData({});
}

float APlayerCharacter::PlayAnimationOf(const TArray<FAnimationData>& Array, const int32 Index, const int32 AttackType) {
	if (this -> bIsAnimationPlaying) {
		return 0.0f;
	}

	if (Index != -1) {
		const auto& [Montage, PlayRate] = Array[Index];
		const float AnimLength = this -> GetMesh() -> GetAnimInstance() -> Montage_Play(Montage, PlayRate);
		if (const int32 ChangedAttackIndex = Index + 1;
			ChangedAttackIndex >= Array.Num()) {
			switch (AttackType) {
				case 0: {
					this -> BasicAttackIndex = 0;
					break;
				}

				case 1: {
					this -> HeavyAttackIndex = 0;
					break;
				}

				case 2: {
					this -> SkillAttackIndex = 0;
					break;
				}

				default: {
					break;
				}
			}
			return AnimLength;
		} else {
			switch (AttackType) {
				case 0: {
					this -> BasicAttackIndex = ChangedAttackIndex;
					break;
				}

				case 1: {
					this -> HeavyAttackIndex = ChangedAttackIndex;
					break;
				}

				case 2: {
					this -> SkillAttackIndex = ChangedAttackIndex;
					break;
				}

				default: {
					break;
				}
			}

			return AnimLength;
		}
	}

	const int32 RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	const auto& [Montage, PlayRate] = Array[RandomIndex];
	const float AnimLength = this -> GetMesh() -> GetAnimInstance() -> Montage_Play(Montage, PlayRate);
	return AnimLength;
}

void APlayerCharacter::ResetCombos() {
	this -> BasicAttackIndex = 0;
	this -> HeavyAttackIndex = 0;
	this -> SkillAttackIndex = 0;
	this -> bIsAnimationPlaying = false;
}

void APlayerCharacter::Die() {

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		for (int32 CurrentIndex = 0; CurrentIndex < this -> InputBindingData.Num(); ++CurrentIndex) {
			const auto& [ActionName, Action, TriggerEvents, FunctionName] = this -> InputBindingData[CurrentIndex];
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
