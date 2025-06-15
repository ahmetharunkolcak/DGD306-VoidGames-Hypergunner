#include "Classes/AnimNotifies/UHitAnimNotify.h"

#include "Classes/Characters/PlayerCharacter.h"

void UUHitAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr) {
		return;
	}

	AActor* Owner = MeshComp -> GetOwner();
	if (Owner == nullptr) {
		return;
	}

	if (APlayerCharacter* Attacker = Cast<APlayerCharacter>(Owner)) {
		Attacker -> TryDealingDamage(this -> DamageForThisAttack);
	}
}
