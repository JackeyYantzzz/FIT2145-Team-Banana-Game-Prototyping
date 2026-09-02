#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveSpawner.generated.h"

/** Configuration for a specific enemy type within a wave */
USTRUCT(BlueprintType)
struct FEnemySubWave
{
	GENERATED_BODY()

	/** The enemy class blueprint to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TSubclassOf<AActor> EnemyClass;

	/** Number of enemies of this type to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 EnemyCount = 5;
};

/** Configuration for a single wave containing multiple sub-waves */
USTRUCT(BlueprintType)
struct FWaveData
{
	GENERATED_BODY()

	/** List of enemy groups spawned sequentially in this wave */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TArray<FEnemySubWave> SubWaves;

	/** Delay between spawning individual enemies (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float SpawnInterval = 1.0f;

	/** Delay before starting the next wave after completing the current one */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float NextWaveDelay = 5.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnWaveChanged,
	int32, CurrentWave,
	int32, TotalWaves
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnWaveCountdownChanged,
	int32, SecondsRemaining
);

UCLASS()
class A1_API AWaveSpawner : public AActor
{
	GENERATED_BODY()

public:
	AWaveSpawner();

	/** Broadcasts the current wave number and total waves */
	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnWaveChanged OnWaveChanged;

	/** Broadcasts the remaining countdown time before the next wave */
	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnWaveCountdownChanged OnWaveCountdownChanged;

	/** Returns 1-based index of the active wave */
	UFUNCTION(BlueprintCallable, Category = "Wave")
	int32 GetCurrentWave() const;

	/** Returns the total configured waves */
	UFUNCTION(BlueprintCallable, Category = "Wave")
	int32 GetTotalWaves() const;

protected:
	virtual void BeginPlay() override;

	/** Array of configured waves */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Setup")
	TArray<FWaveData> Waves;

	/** Reference spline actor used for enemy pathfinding */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Setup")
	AActor* SplinePathRef;

private:
	/** Index of the active wave */
	int32 CurrentWaveIndex;

	/** Index of the active sub-wave group */
	int32 CurrentSubWaveIndex;

	/** Count of enemies spawned in the active sub-wave */
	int32 SpawnedInCurrentSubWave;

	/** Tracked countdown seconds before the next wave begins */
	float WaveCountdownRemaining = 0.0f;

	FTimerHandle TimerHandle_SpawnEnemy;
	FTimerHandle TimerHandle_NextWave;
	FTimerHandle TimerHandle_WaveCountdown;

	/** Starts spawning for the current wave */
	void StartNextWave();

	/** Spawns a single enemy and handles sub-wave progression */
	void SpawnSingleEnemy();

	/** Cleans up active wave timer and initiates countdown for next wave */
	void OnWaveCompleted();

	/** Updates the countdown timer display */
	void UpdateWaveCountdown();
};