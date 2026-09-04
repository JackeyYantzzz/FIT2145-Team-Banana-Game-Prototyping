#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveSpawner.generated.h"

USTRUCT(BlueprintType)
struct FEnemySubWave
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TSubclassOf<AActor> EnemyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    int32 EnemyCount = 5;
};

USTRUCT(BlueprintType)
struct FWaveData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<FEnemySubWave> SubWaves;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    float SpawnInterval = 1.0f;

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
    FOnAllWavesCompleted
);

UCLASS()
class A1_API AWaveSpawner : public AActor
{
    GENERATED_BODY()

public:
    AWaveSpawner();

    UPROPERTY(BlueprintAssignable, Category = "Wave")
    FOnWaveChanged OnWaveChanged;

    UPROPERTY(BlueprintAssignable, Category = "Wave")
    FOnWaveCountdownChanged OnWaveCountdownChanged;

    UPROPERTY(BlueprintAssignable, Category = "Wave")
    FOnAllWavesCompleted OnAllWavesCompleted;

    UFUNCTION(BlueprintCallable, Category = "Wave")
    int32 GetCurrentWave() const;

    UFUNCTION(BlueprintCallable, Category = "Wave")
    int32 GetTotalWaves() const;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Setup")
    TArray<FWaveData> Waves;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Setup")
    AActor* SplinePathRef;

private:
    int32 CurrentWaveIndex;

    int32 CurrentSubWaveIndex;

    int32 SpawnedInCurrentSubWave;

    float WaveCountdownRemaining = 0.0f;

    FTimerHandle TimerHandle_SpawnEnemy;
    FTimerHandle TimerHandle_NextWave;
    FTimerHandle TimerHandle_WaveCountdown;
    FTimerHandle TimerHandle_CheckVictory;

    void StartNextWave();

    void SpawnSingleEnemy();

    void OnWaveCompleted();

    void UpdateWaveCountdown();

    void CheckForVictory();
};