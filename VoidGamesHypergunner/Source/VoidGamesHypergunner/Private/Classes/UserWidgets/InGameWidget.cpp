#include "Classes/UserWidgets/InGameWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interfaces/HealthComponentContainable.h"

void UInGameWidget::SetTimer(const float Time) const {
	if (this -> TimeValue != nullptr) {
		const FText FormattedTime = this -> GetFormattedTime(Time);
		this -> TimeValue -> SetText(FormattedTime);
	}
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
		this -> HealthBarL -> SetPercent(CharacterHealthRate);
	} else {
		this -> HealthBarR -> SetPercent(CharacterHealthRate);
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
