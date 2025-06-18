#include "Classes/AnimNotifies/HitAnimNotify.h"

#include "Classes/Characters/PlayerCharacter.h"

void UHitAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr) {
		return;
	}

	AActor* Owner = MeshComp -> GetOwner();
	if (Owner == nullptr) {
		return;
	}

	if (APlayerCharacter* Attacker = Cast<APlayerCharacter>(Owner)) {
		Attacker -> TryDealingDamage(this -> DamageForThisAttack, this -> bPlaySound);
	}
}
