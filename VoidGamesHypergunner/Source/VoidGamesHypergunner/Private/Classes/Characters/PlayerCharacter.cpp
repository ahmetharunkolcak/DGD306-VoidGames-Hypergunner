#include "VoidGamesHypergunner/Public/Classes/Characters/PlayerCharacter.h"

#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Classes/ActorComponents/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

void APlayerCharacter::TryDealingDamage(const float Amount, const bool bShouldPlaySound) {
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * this -> AttackRange;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	if (bShouldPlaySound) {
		if (const FSoundListData* AttackingSounds = this -> FindSoundsByName("Attack");
		   AttackingSounds != nullptr) {
			this -> PlaySoundOf(AttackingSounds -> Sounds, -1);
		   } else {
		   	UE_LOG(LogTemp,
					  Warning,
					  TEXT("APlayerCharacter::TryDealingDamage: AttackingSounds has no sound data."));
		   }
	}

	if (FHitResult HitResult;
		GetWorld() -> LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, CollisionQueryParams)) {
		if (AActor* HitActor = HitResult.GetActor()) {
			UGameplayStatics::ApplyDamage(HitActor, Amount, this -> GetController(), this, UDamageType::StaticClass());
		}
	}
}

float APlayerCharacter::TakeDamage(const float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (this -> bDidFinishHimTriggered) {
		return ActualDamage;
	}

	if (this -> bDidTheCharacterDie) {
		if (APlayerCharacter* WinnerPlayer = Cast<APlayerCharacter>(DamageCauser);
			WinnerPlayer != nullptr) {
			WinnerPlayer -> Victory();
			this -> Defeat();
		}

		if (const FAnimationListData* DefeatAnimationList = this -> FindAnimationsByName(FName("Defeat"));
			DefeatAnimationList != nullptr) {
			const TArray<FAnimationData>& Montages = DefeatAnimationList->Montages;
			this -> PlayAnimationOf(Montages, -1, -1);
		} else {
			UE_LOG(LogTemp,
			       Warning,
			       TEXT("APlayerCharacter::TakeDamage: DefeatAnimationList is nullptr for Loser Player!"));
		}

		return ActualDamage;
	}

	if (this -> HealthComponent != nullptr) {
		this -> HealthComponent -> UpdateHealth(ActualDamage, true, false);
		this -> OnHealthChanged.Broadcast(this -> PlayerIndex);

		if (this -> HealthComponent -> GetCurrentHealth() <= 0.0f) {
			if (const APlayerCharacter* WinnerPlayer = Cast<APlayerCharacter>(DamageCauser);
				WinnerPlayer != nullptr) {
				WinnerPlayer -> GetMesh() -> GetAnimInstance() -> StopAllMontages(0.5f);
				this -> GetMesh() -> GetAnimInstance() -> StopAllMontages(0.5f);

				if (UCharacterMovementComponent* WinnerPlayerMovementComponent = WinnerPlayer -> GetCharacterMovement();
					WinnerPlayerMovementComponent != nullptr) {
					WinnerPlayerMovementComponent -> DisableMovement();
				} else {
					UE_LOG(LogTemp,
					       Warning,
					       TEXT("APlayerCharacter::TakeDamage: Tried to disable movement for Winner Player but WinnerPlayerMovementComponent was null!"))
				}

				if (UCharacterMovementComponent* LoserPlayerMovementComponent = this -> GetCharacterMovement();
					LoserPlayerMovementComponent != nullptr) {
					LoserPlayerMovementComponent -> DisableMovement();
				} else {
					UE_LOG(LogTemp,
						   Warning,
						   TEXT("APlayerCharacter::TakeDamage: Tried to disable movement for Loser Player but LoserPlayerMovementComponent was null!"))
				}
			}

			this -> Die();
		} else {
			this -> GetHit();
		}

		if (const FSoundListData* GettingHitSounds = this -> FindSoundsByName("GettingHit");
			GettingHitSounds != nullptr) {
			this -> PlaySoundOf(GettingHitSounds -> Sounds, -1);
		} else {
			UE_LOG(LogTemp,
			       Warning,
			       TEXT("APlayerCharacter::TakeDamage: GettingHitSounds has no sound data."));
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
	if (this -> bDidTheCharacterDie || this -> bShouldBlockInput) {
		return;
	}

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
	if (this -> bDidTheCharacterDie || this -> bShouldBlockInput) {
		return;
	}

	if (!this -> bIsAnimationPlaying) {
		if (const FAnimationListData* NormalAttackAnimationList = this -> FindAnimationsByName(FName("NormalAttack"));
			NormalAttackAnimationList != nullptr) {
			const TArray<FAnimationData>& Montages = NormalAttackAnimationList -> Montages;
			this -> PlayAnimationOf(Montages, this -> NormalAttackIndex, 0);
		} else {
			UE_LOG(LogTemp,
			       Warning,
			       TEXT("APlayerCharacter::DoNormalAttack: NormalAttackAnimationList is nullptr!"));
		}
	}
}

void APlayerCharacter::DoHeavyAttack(const FInputActionValue& Value) {
	if (this -> bDidTheCharacterDie || this -> bShouldBlockInput) {
		return;
	}

	if (!this -> bIsAnimationPlaying) {
		if (const FAnimationListData* HeavyAttackAnimationList = this -> FindAnimationsByName(FName("HeavyAttack"));
			HeavyAttackAnimationList != nullptr) {
			const TArray<FAnimationData>& Montages = HeavyAttackAnimationList -> Montages;
			this -> PlayAnimationOf(Montages, this -> HeavyAttackIndex, 1);
		} else {
			UE_LOG(LogTemp,
				   Warning,
				   TEXT("APlayerCharacter::DoHeavyAttack: HeavyAttackAnimationList is nullptr!"));
		}
	}
}

void APlayerCharacter::DoSkillAttack(const FInputActionValue& Value) {
	if (this -> bDidTheCharacterDie || this -> bShouldBlockInput) {
		return;
	}

	if (!this -> bIsAnimationPlaying) {
		if (const FAnimationListData* SkillAttackAnimationList = this -> FindAnimationsByName(FName("SkillAttack"));
			SkillAttackAnimationList != nullptr) {
			const TArray<FAnimationData>& Montages = SkillAttackAnimationList -> Montages;
			this -> PlayAnimationOf(Montages, this -> SkillAttackIndex, 2);
		} else {
			UE_LOG(LogTemp,
				   Warning,
				   TEXT("APlayerCharacter::DoSkillAttack: SkillAttackAnimationList is nullptr!"));
		}
	}
}

void APlayerCharacter::Dodge(const FInputActionValue& Value) {
	if (this -> bDidTheCharacterDie || this -> bShouldBlockInput) {
		return;
	}

	if (!this -> bIsAnimationPlaying) {
		if (const FAnimationListData* DodgeAnimationList = this -> FindAnimationsByName(FName("Dodge"));
			DodgeAnimationList != nullptr) {
			const TArray<FAnimationData>& Montages = DodgeAnimationList -> Montages;
			this -> PlayAnimationOf(Montages, -1, -1);

			const FSoundListData* DodgeSounds = this -> FindSoundsByName("Dodge");
			if (DodgeSounds != nullptr) {
				this -> PlaySoundOf(DodgeSounds -> Sounds, -1);
			} else {
				UE_LOG(LogTemp,
					   Warning,
					   TEXT("APlayerCharacter::Dodge: DodgeSounds has no sound data."));
			}
		} else {
			UE_LOG(LogTemp,
				   Warning,
				   TEXT("APlayerCharacter::Dodge: DodgeAnimationList is nullptr!"));
		}
	}
}
void APlayerCharacter::GetHit() {
	if (this -> bDidTheCharacterDie || this -> bShouldBlockInput) {
		return;
	}

	if (const FAnimationListData* GetHitAnimationList = this -> FindAnimationsByName(FName("GetHit"));
		GetHitAnimationList != nullptr) {
		const TArray<FAnimationData>& Montages = GetHitAnimationList -> Montages;
		this -> PlayAnimationOf(Montages, -1, -1);
	} else {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("APlayerCharacter::GetHit: GetHitAnimationList is nullptr!"));
	}
}

void APlayerCharacter::Victory() {
	this -> bShouldBlockInput = true;

	if (const FAnimationListData* VictoryAnimationList = this -> FindAnimationsByName(FName("Victory"));
		VictoryAnimationList != nullptr) {
		const TArray<FAnimationData>& Montages = VictoryAnimationList -> Montages;
		this -> PlayAnimationOf(Montages, -1, -1);
	} else {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("APlayerCharacter::Dodge: VictoryAttackAnimationList is nullptr!"));
	}
}

void APlayerCharacter::Defeat() {
	this -> bShouldBlockInput = true;

	if (const FAnimationListData* DefeatAnimationList = this -> FindAnimationsByName(FName("Defeat"));
		DefeatAnimationList != nullptr) {
		const TArray<FAnimationData>& Montages = DefeatAnimationList -> Montages;
		this -> PlayAnimationOf(Montages, -1, -1);
	} else {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("APlayerCharacter::Dodge: DefeatAnimationList is nullptr!"));
	}

	if (const FSoundListData* GettingHitSounds = this -> FindSoundsByName("GettingHit");
		GettingHitSounds != nullptr) {
		this -> PlaySoundOf(GettingHitSounds->Sounds, -1);
	} else {
		UE_LOG(LogTemp,
		       Warning,
		       TEXT("APlayerCharacter::TakeDamage: GettingHitSounds has no sound data."));
	}
}

const FAnimationListData* APlayerCharacter::FindAnimationsByName(const FName Name) const {
	if (const FAnimationListData* FoundAnimationData = this -> AnimationData.Find(Name);
		FoundAnimationData != nullptr) {
		return FoundAnimationData;
	}

	return nullptr;
}

const FSoundListData* APlayerCharacter::FindSoundsByName(const FName Name) const {
	if (const FSoundListData* FoundSoundData = this -> SoundData.Find(Name);
		FoundSoundData != nullptr) {
		return FoundSoundData;
	}

	return nullptr;
}

void APlayerCharacter::PlayAnimationOf(const TArray<FAnimationData>& Array, const int32 Index, const int32 AttackType) {
	if (Array.Num() == 0) {
		UE_LOG(LogTemp,
			Warning,
			TEXT("APlayerCharacter::PlayAnimationOf: Array contains no animation data."));

		return;
	}

	this -> bIsAnimationPlaying = true;

	if (Index != -1) {
		const auto& [Montage, PlayRate] = Array[Index];
		const float AnimLength = this -> GetMesh() -> GetAnimInstance() -> Montage_Play(Montage, PlayRate);
		if (const int32 ChangedAttackIndex = Index + 1;
			ChangedAttackIndex >= Array.Num()) {
			switch (AttackType) {
				case 0: {
					this -> NormalAttackIndex = 0;
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
		} else {
			switch (AttackType) {
				case 0: {
					this -> NormalAttackIndex = ChangedAttackIndex;
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
		}

		GetWorld() -> GetTimerManager().SetTimer(
			this -> AnimationCountdownTimerHandler,
			this,
			&APlayerCharacter::ResetAnimationPlayState,
			(AnimLength / PlayRate) * this -> AnimationCutoffRate,
			false
		);

		GetWorld() -> GetTimerManager().SetTimer(
			this -> ResetComboTimerHandler,
			this,
			&APlayerCharacter::ResetCombos,
			AnimLength,
			false
		);

		return;
	}

	const int32 RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	const auto& [Montage, PlayRate] = Array[RandomIndex];
	const float AnimLength = this -> GetMesh() -> GetAnimInstance() -> Montage_Play(Montage, PlayRate);

	GetWorld() -> GetTimerManager().SetTimer(
		this -> AnimationCountdownTimerHandler,
		this,
		&APlayerCharacter::ResetAnimationPlayState,
		(AnimLength / PlayRate) * this -> AnimationCutoffRate,
		false
	);

	GetWorld() -> GetTimerManager().SetTimer(
		this -> ResetComboTimerHandler,
		this,
		&APlayerCharacter::ResetCombos,
		AnimLength,
		false
	);
}

void APlayerCharacter::PlaySoundOf(const TArray<FSoundData>& Array, const int32 Index) const {
	if (Array.Num() == 0) {
		UE_LOG(LogTemp,
			Warning,
			TEXT("APlayerCharacter::PlaySoundOf: Array contains no sound data."));

		return;
	}

	if (Index != -1) {
		const auto& [Sound, MinVolume, MaxVolume, MinPitch, MaxPitch] = Array[Index];
		const float Volume = FMath::RandRange(MinVolume, MaxVolume);
		const float Pitch = FMath::RandRange(MinPitch, MaxPitch);
		UGameplayStatics::PlaySound2D(GetWorld(), Sound, Volume, Pitch);
		return;
	}

	const int32 RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	const auto& [Sound, MinVolume, MaxVolume, MinPitch, MaxPitch] = Array[RandomIndex];
	const float Volume = FMath::RandRange(MinVolume, MaxVolume);
	const float Pitch = FMath::RandRange(MinPitch, MaxPitch);
	UGameplayStatics::PlaySound2D(GetWorld(), Sound, Volume, Pitch);
}

void APlayerCharacter::ResetCombos() {
	this -> NormalAttackIndex = 0;
	this -> HeavyAttackIndex = 0;
	this -> SkillAttackIndex = 0;
}

void APlayerCharacter::ResetAnimationPlayState() {
	this -> bIsAnimationPlaying = false;
}

void APlayerCharacter::Die() {
	if (this -> bDidTheCharacterDie) {
		return;
	}

	this -> bDidTheCharacterDie = true;

	if (const FSoundListData* FinishHimSounds = this -> FindSoundsByName("FinishHim");
		FinishHimSounds != nullptr) {
		this -> PlaySoundOf(FinishHimSounds -> Sounds, -1);
	} else {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("APlayerCharacter::Die: FinishHimSounds has no sound data."));
	}

	this -> OnDeath.Broadcast(this -> PlayerIndex);
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
