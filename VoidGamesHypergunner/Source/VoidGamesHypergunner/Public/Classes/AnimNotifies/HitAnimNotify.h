#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "HitAnimNotify.generated.h"

UCLASS()
class VOIDGAMESHYPERGUNNER_API UHitAnimNotify : public UAnimNotify {
	GENERATED_BODY()

	public:
		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	protected:
		UPROPERTY(EditAnywhere, Category = "Custom|Damage")
		float DamageForThisAttack = 0.0f;
};
