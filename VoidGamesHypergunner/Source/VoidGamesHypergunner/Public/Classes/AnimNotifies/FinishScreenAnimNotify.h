#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FinishScreenAnimNotify.generated.h"

UCLASS()
class VOIDGAMESHYPERGUNNER_API UFinishScreenAnimNotify : public UAnimNotify {
	GENERATED_BODY()

	public:
		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
