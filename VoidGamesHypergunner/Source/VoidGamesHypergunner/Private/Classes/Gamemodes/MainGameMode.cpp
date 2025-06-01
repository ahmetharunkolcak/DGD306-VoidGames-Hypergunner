#include "VoidGamesHypergunner/Public/Classes/Gamemodes/MainGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AMainGameMode::BeginPlay() {
	Super::BeginPlay();

	if (UWorld* CurrentWorld = GetWorld()) {
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(CurrentWorld, APlayerStart::StaticClass(), PlayerStarts);
		for (int32 CurrentIndex = 0; CurrentIndex < PlayerStarts.Num(); ++CurrentIndex) {
			const AActor* CurrentPlayerStart = PlayerStarts[CurrentIndex];
			const FTransform CurrentPlayerStartTransform = CurrentPlayerStart -> GetTransform();
			
			TSubclassOf<APlayerCharacter> SpawningActorClass = nullptr;
			if (this -> PlayerCharacters.IsValidIndex(CurrentIndex)) {
				SpawningActorClass = this -> PlayerCharacters[CurrentIndex];
			}

			if (SpawningActorClass == nullptr) {
				UE_LOG(LogTemp,
				       Warning,
				       TEXT("AMainGameMode::BeginPlay: Spawning Actor with Index %d could not found: Class is null"),
				       CurrentIndex);

				continue;
			}

			if (CurrentIndex == 0) {
				AActor* SpawnedPlayerActor = CurrentWorld -> SpawnActor(SpawningActorClass, &CurrentPlayerStartTransform);
				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				APawn* PlayerPawn = Cast<APawn>(SpawnedPlayerActor);
				PlayerController -> Possess(PlayerPawn);
			} else {
				UGameInstance* CurrentGameInstance = GetGameInstance();

				FString OutErrorMessage;
				if (const ULocalPlayer* CreatedLocalPlayer = CurrentGameInstance -> CreateLocalPlayer(-1, OutErrorMessage, true)) {
					AActor* SpawnedPlayerActor = CurrentWorld -> SpawnActor(SpawningActorClass, &CurrentPlayerStartTransform);
					APlayerController* PlayerController = CreatedLocalPlayer -> GetPlayerController(CurrentWorld);
					APawn* PlayerPawn = Cast<APawn>(SpawnedPlayerActor);
					PlayerController -> Possess(PlayerPawn);
				} else {
					UE_LOG(LogTemp,
					       Warning,
					       TEXT("AMainGameMode::BeginPlay - Failed to create local player: %s"), *OutErrorMessage);
				}
			}
		}
	}
}
