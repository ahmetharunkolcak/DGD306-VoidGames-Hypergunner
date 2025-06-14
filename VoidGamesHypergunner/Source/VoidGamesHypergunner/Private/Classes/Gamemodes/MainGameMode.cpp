#include "VoidGamesHypergunner/Public/Classes/Gamemodes/MainGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "Classes/Actors/SpawnPoint.h"

AMainGameMode::AMainGameMode() {
	PrimaryActorTick.bCanEverTick = true;
}

void AMainGameMode::BeginPlay() {
	Super::BeginPlay();

	if (UWorld* CurrentWorld = GetWorld()) {
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(CurrentWorld, ASpawnPoint::StaticClass(), PlayerStarts);
		for (int32 CurrentIndex = 0; CurrentIndex < PlayerStarts.Num(); ++CurrentIndex) {
			AActor* CurrentPlayerStart = PlayerStarts[CurrentIndex];
			if (!CurrentPlayerStart -> Implements<USpawnable>()) {
				UE_LOG(LogTemp,
				       Warning,
				       TEXT("AMainGameMode::BeginPlay: Could not verified Spawnable interface existence on spawn point actor with index %d"),
				       CurrentIndex);
				continue;
			}

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

			const ISpawnable* SpawnableInterface = Cast<ISpawnable>(CurrentPlayerStart);
			const int32 SpawnIndex = SpawnableInterface -> GetSpawnIndex();
			if (this -> SpawnedIndexes.Contains(SpawnIndex)) {
				UE_LOG(LogTemp,
					Warning,
					TEXT("AMainGameMode::BeginPlay: Already spawned a player with index %d"),
					SpawnIndex);
				continue;
			}
			if (SpawnIndex == 0) {
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
					       TEXT("AMainGameMode::BeginPlay: Failed to create local player: %s"), *OutErrorMessage);
				}
			}

			if (CurrentPlayerStart != nullptr && !CurrentPlayerStart -> IsPendingKillPending()) {
				CurrentPlayerStart -> Destroy();
				this -> SpawnedIndexes.Add(SpawnIndex);
			}
		}
	}

	this -> CurrentTime = this -> GameTime;
}

void AMainGameMode::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	this -> CurrentTime -= DeltaSeconds;
}

float AMainGameMode::GetGameplayTime(const bool bIsCurrentTime) const {
	return bIsCurrentTime ? this -> CurrentTime : this -> GameTime;
}
