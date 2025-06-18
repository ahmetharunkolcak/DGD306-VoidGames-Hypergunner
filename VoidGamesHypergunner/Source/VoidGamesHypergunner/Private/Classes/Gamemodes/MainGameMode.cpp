#include "VoidGamesHypergunner/Public/Classes/Gamemodes/MainGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "Classes/Actors/SpawnPoint.h"
#include "Classes/CameraActors/SideViewCameraActor.h"
#include "Classes/GameInstances/MainGameInstance.h"
#include "Classes/HUDs/MainHUD.h"
#include "GameFramework/HUD.h"
#include "Interfaces/WidgetContainable.h"

AMainGameMode::AMainGameMode() {
	PrimaryActorTick.bCanEverTick = true;
}

void AMainGameMode::BeginPlay() {
	Super::BeginPlay();

	if (UWorld* CurrentWorld = GetWorld()) {
		TArray<AActor*> PlayerStarts;
		TArray<FCharacterSelectionData>& PlayerCharactersData = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())) -> GetSelectedPlayerCharactersData();
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
			if (PlayerCharactersData.IsValidIndex(CurrentIndex)) {
				SpawningActorClass = PlayerCharactersData[CurrentIndex].Character3D.Get();
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
				Cast<APlayerCharacter>(SpawnedPlayerActor) -> SetPlayerIndex(SpawnIndex);
				Cast<APlayerCharacter>(SpawnedPlayerActor) -> OnDeath.AddDynamic(this, &AMainGameMode::HandleDeath);
				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				APawn* PlayerPawn = Cast<APawn>(SpawnedPlayerActor);
				PlayerController -> Possess(PlayerPawn);

				AHUD* HUD = PlayerController -> GetHUD();
				if (HUD -> Implements<UWidgetContainable>()) {
					IWidgetContainable* WidgetContainableInterface = Cast<IWidgetContainable>(HUD);
					WidgetContainableInterface -> SetInGameCharacterImage(PlayerCharactersData[CurrentIndex].Character2D, true);
				}
			} else {
				UGameInstance* CurrentGameInstance = GetGameInstance();

				FString OutErrorMessage;
				if (const ULocalPlayer* CreatedLocalPlayer = CurrentGameInstance -> CreateLocalPlayer(-1, OutErrorMessage, true)) {
					AActor* SpawnedPlayerActor = CurrentWorld -> SpawnActor(SpawningActorClass, &CurrentPlayerStartTransform);
					Cast<APlayerCharacter>(SpawnedPlayerActor) -> SetPlayerIndex(SpawnIndex);
					Cast<APlayerCharacter>(SpawnedPlayerActor) -> OnDeath.AddDynamic(this, &AMainGameMode::HandleDeath);
					APlayerController* PlayerController = CreatedLocalPlayer -> GetPlayerController(CurrentWorld);
					APawn* PlayerPawn = Cast<APawn>(SpawnedPlayerActor);
					PlayerController -> Possess(PlayerPawn);

					AHUD* HUD = UGameplayStatics::GetPlayerController(GetWorld(), 0) -> GetHUD();
					if (HUD -> Implements<UWidgetContainable>()) {
						IWidgetContainable* WidgetContainableInterface = Cast<IWidgetContainable>(HUD);
						WidgetContainableInterface -> SetInGameCharacterImage(PlayerCharactersData[SpawnIndex].Character2D, false);
					}
				} else {
					UE_LOG(LogTemp,
					       Warning,
					       TEXT("AMainGameMode::BeginPlay: Failed to create local player: %s"), *OutErrorMessage);
				}
			}

			this -> SpawnedIndexes.Add(SpawnIndex);
		}
	}

	this -> CurrentTime = this -> GameTime;

	if (const auto& [Sound, MinVolume, MaxVolume, MinPitch, MaxPitch] = BackgroundMusic;
		Sound != nullptr) {
		const float Volume = FMath::Min(MinVolume, MaxVolume);
		const float Pitch = FMath::Min(MinPitch, MaxPitch);
		UGameplayStatics::PlaySound2D(GetWorld(), Sound, Volume, Pitch);
	} else {
		UE_LOG(LogTemp,
		       Warning,
		       TEXT("AMainGameMode::BeginPlay: Failed to play background music, Sound is null!"));
	}
}

void AMainGameMode::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	this -> CurrentTime -= DeltaSeconds;
	if (this -> CurrentTime <= 0.0f) {
		this -> CurrentTime = 0.0f;
		Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0) -> GetHUD()) -> TriggerReturnScreen(true);
		SetActorTickEnabled(false);
	}
}

float AMainGameMode::GetGameplayTime(const bool bIsCurrentTime) const {
	return bIsCurrentTime ? this -> CurrentTime : this -> GameTime;
}

void AMainGameMode::HandleDeath(int32 PlayerIndex) {
	SetActorTickEnabled(false);
}
