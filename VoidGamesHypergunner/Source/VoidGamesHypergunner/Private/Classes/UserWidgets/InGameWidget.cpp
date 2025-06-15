#include "Classes/UserWidgets/InGameWidget.h"

#include "Classes/Characters/PlayerCharacter.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interfaces/HealthComponentContainable.h"

void UInGameWidget::SetTimer(const float Time) const {
	if (this -> TimeValue != nullptr) {
		const FText FormattedTime = this -> GetFormattedTime(Time);
		this -> TimeValue -> SetText(FormattedTime);
	}
}

void UInGameWidget::SetLeftImage(UTexture2D* Image) {
	if (Image == nullptr) {
		UE_LOG(LogTemp,
			Warning,
			TEXT("UInGameWidget::SetLeftImage: Image is nullptr!"));

		return;
	}

	this -> ImageL -> SetBrushFromTexture(Image);
}

void UInGameWidget::SetRightImage(UTexture2D* Image) {
	if (Image == nullptr) {
		UE_LOG(LogTemp,
			Warning,
			TEXT("UInGameWidget::SetRightImage: Image is nullptr!"));

		return;
	}

	this -> ImageR -> SetBrushFromTexture(Image);
}

void UInGameWidget::UpdateHealthFor(const AActor* Player, const bool bIsLeftPlayer) {
	if (!Player -> Implements<UHealthComponentContainable>()) {
		UE_LOG(LogTemp,
		       Warning,
		       TEXT("UInGameWidget::UpdateHealthFor: Actor that does not implement IHealthComponentContainable interface is given!"))
		return;
	}

	const IHealthComponentContainable* HealthComponentContainableInterface = Cast<IHealthComponentContainable>(Player);
	const float CharacterHealthRate = HealthComponentContainableInterface -> GetCharacterHealthRate();

	if (bIsLeftPlayer) {
		this -> AnimTimeForHealthBarL = 0.0f;
		this -> bIsUpdatingLeftHealthBar = true;
		this -> TargetHealthForBarL = CharacterHealthRate;
	} else {
		this -> AnimTimeForHealthBarR = 0.0f;
		this -> bIsUpdatingRightHealthBar = true;
		this -> TargetHealthForBarR = CharacterHealthRate;
	}
}

void UInGameWidget::SetupPlayerListeners(const TArray<AActor*>& Players) {
	for (AActor* Player : Players) {
		this -> TrackedPlayers.Add(Cast<APlayerCharacter>(Player));
	}

	for (APlayerCharacter* Player : this -> TrackedPlayers) {
		if (Player != nullptr) {
			Player -> OnHealthChanged.AddDynamic(this, &UInGameWidget::HandleHealthChanged);
			Player -> OnDeath.AddDynamic(this, &UInGameWidget::HandleDeath);
		}
	}
}

void UInGameWidget::HandleHealthChanged(const int32 PlayerIndex) {
	for (const APlayerCharacter* Player : this -> TrackedPlayers) {
		if (Player != nullptr && Player -> GetPlayerIndex() == PlayerIndex) {
			const bool bIsLeftPlayer = (PlayerIndex == 0);
			this -> UpdateHealthFor(Player, bIsLeftPlayer);
			return;
		}
	}
}

void UInGameWidget::HandleDeath(int32 PlayerIndex) {
	if (this -> FadeOutAnimation != nullptr) {
		PlayAnimation(this -> FadeOutAnimation);
	} else {
		UE_LOG(LogTemp,
		       Warning,
		       TEXT("UInGameWidget::HandleDeath: FadeOutAnimation is nullptr!"));
	}
}

FText UInGameWidget::GetFormattedTime(float Time) const {
	const int32 Minutes = FMath::Floor(Time / 60.0f);
	Time -= Minutes * 60.0f;
	const int32 Seconds = FMath::Floor(Time);

	FNumberFormattingOptions FormatOptionsForMinute;
	FormatOptionsForMinute.SetMinimumIntegralDigits(1);
	FormatOptionsForMinute.SetMaximumIntegralDigits(1);
	FormatOptionsForMinute.SetMinimumFractionalDigits(0);
	FormatOptionsForMinute.SetMaximumFractionalDigits(0);

	FNumberFormattingOptions FormatOptionsForSeconds;
	FormatOptionsForSeconds.SetMinimumIntegralDigits(2);
	FormatOptionsForSeconds.SetMaximumIntegralDigits(2);
	FormatOptionsForSeconds.SetMinimumFractionalDigits(0);
	FormatOptionsForSeconds.SetMaximumFractionalDigits(0);

	FFormatNamedArguments Args;
	Args.Add(TEXT("Minutes"), FText::AsNumber(Minutes, &FormatOptionsForMinute));
	Args.Add(TEXT("Seconds"), FText::AsNumber(Seconds, &FormatOptionsForSeconds));

	const FTextFormat TextFormat = FText::FromString(TEXT("{Minutes}:{Seconds}"));
	const FText FormattedText = FText::Format(TextFormat, Args);

	return FormattedText;
}

void UInGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsUpdatingLeftHealthBar && !bIsUpdatingRightHealthBar) {
		return;
	}

	if (bIsUpdatingLeftHealthBar) {
		if (FMath::IsNearlyEqual(this -> AnimTimeForHealthBarL, this -> HealthBarUpdateAnimationTime, 0.001f)) {
			this -> HealthBarL -> SetPercent(this -> TargetHealthForBarL);
			this -> AnimTimeForHealthBarL = this -> HealthBarUpdateAnimationTime;
			this -> bIsUpdatingLeftHealthBar = false;
		} else {
			this -> AnimTimeForHealthBarL += InDeltaTime;
			float Alpha = FMath::Clamp(this -> AnimTimeForHealthBarL / this -> HealthBarUpdateAnimationTime, 0.0f, 1.0f);
			const float DisplayedPercentageForL = this -> HealthBarL -> GetPercent();
			const float NewBarPercentValue = FMath::Lerp(DisplayedPercentageForL, this -> TargetHealthForBarL, Alpha);
			this -> HealthBarL -> SetPercent(NewBarPercentValue);

			if (FMath::IsNearlyEqual(Alpha, 1.0f)) {
				this -> HealthBarL -> SetPercent(this -> TargetHealthForBarL);
				this -> AnimTimeForHealthBarL = this -> HealthBarUpdateAnimationTime;
				this -> bIsUpdatingLeftHealthBar = false;
			}
		}
	}

	if (bIsUpdatingRightHealthBar) {
		if (FMath::IsNearlyEqual(this -> AnimTimeForHealthBarR, this -> HealthBarUpdateAnimationTime, 0.001f)) {
			this -> HealthBarR -> SetPercent(this -> TargetHealthForBarR);
			this -> AnimTimeForHealthBarR = this -> HealthBarUpdateAnimationTime;
			this -> bIsUpdatingRightHealthBar = false;
		} else {
			this -> AnimTimeForHealthBarR += InDeltaTime;
			const float RawAlpha = this -> AnimTimeForHealthBarR / this -> HealthBarUpdateAnimationTime;
			const float Alpha = FMath::Clamp(FMath::InterpEaseInOut(0.0f, 1.0f, RawAlpha, this -> AnimationInterpolationSpeed), 0.0f, 1.0f);

			const float DisplayedPercentageForR = this -> HealthBarR -> GetPercent();
			const float NewBarPercentValue = FMath::Lerp(DisplayedPercentageForR, this -> TargetHealthForBarR, Alpha);
			this -> HealthBarR -> SetPercent(NewBarPercentValue);

			if (FMath::IsNearlyEqual(Alpha, 1.0f)) {
				this -> HealthBarR -> SetPercent(this -> TargetHealthForBarR);
				this -> AnimTimeForHealthBarR = this -> HealthBarUpdateAnimationTime;
				this -> bIsUpdatingRightHealthBar = false;
			}
		}
	}
}
