#include "WaveSpawner.h"
#include "EnemyBaseCpp.h"
#include "CoffinEndPoint.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "TimerManager.h"

AWaveSpawner::AWaveSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    CurrentWaveIndex = 0;
    CurrentSubWaveIndex = 0;
    SpawnedInCurrentSubWave = 0;

    WaveCountdownRemaining = 0.0f;
    SplinePathRef = nullptr;
}

void AWaveSpawner::BeginPlay()
{
    Super::BeginPlay();

    StartNextWave();
}

void AWaveSpawner::StartNextWave()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_WaveCountdown);
    OnWaveCountdownChanged.Broadcast(0);

    if (!Waves.IsValidIndex(CurrentWaveIndex))
    {
        return;
    }

    CurrentSubWaveIndex = 0;
    SpawnedInCurrentSubWave = 0;

    OnWaveChanged.Broadcast(
        CurrentWaveIndex + 1,
        Waves.Num()
    );

    const FWaveData& CurrentWave = Waves[CurrentWaveIndex];

    GetWorldTimerManager().SetTimer(
        TimerHandle_SpawnEnemy,
        this,
        &AWaveSpawner::SpawnSingleEnemy,
        CurrentWave.SpawnInterval,
        true
    );
}

void AWaveSpawner::SpawnSingleEnemy()
{
    if (!Waves.IsValidIndex(CurrentWaveIndex))
    {
        return;
    }

    const FWaveData& CurrentWave = Waves[CurrentWaveIndex];

    while (CurrentWave.SubWaves.IsValidIndex(CurrentSubWaveIndex))
    {
        const FEnemySubWave& CurrentSubWave =
            CurrentWave.SubWaves[CurrentSubWaveIndex];

        if (SpawnedInCurrentSubWave < CurrentSubWave.EnemyCount)
        {
            if (CurrentSubWave.EnemyClass && GetWorld())
            {
                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride =
                    ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

                GetWorld()->SpawnActor<AActor>(
                    CurrentSubWave.EnemyClass,
                    GetActorTransform(),
                    SpawnParams
                );
            }

            SpawnedInCurrentSubWave++;
            return;
        }

        CurrentSubWaveIndex++;
        SpawnedInCurrentSubWave = 0;
    }

    OnWaveCompleted();
}

void AWaveSpawner::OnWaveCompleted()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_SpawnEnemy);

    if (!Waves.IsValidIndex(CurrentWaveIndex))
    {
        return;
    }

    const float Delay = Waves[CurrentWaveIndex].NextWaveDelay;

    CurrentWaveIndex++;

    if (!Waves.IsValidIndex(CurrentWaveIndex))
    {
        OnWaveCountdownChanged.Broadcast(0);

        GetWorldTimerManager().SetTimer(
            TimerHandle_CheckVictory,
            this,
            &AWaveSpawner::CheckForVictory,
            0.5f,
            true
        );

        return;
    }

    WaveCountdownRemaining = Delay;

    OnWaveCountdownChanged.Broadcast(
        FMath::CeilToInt(WaveCountdownRemaining)
    );

    GetWorldTimerManager().SetTimer(
        TimerHandle_WaveCountdown,
        this,
        &AWaveSpawner::UpdateWaveCountdown,
        1.0f,
        true
    );

    GetWorldTimerManager().SetTimer(
        TimerHandle_NextWave,
        this,
        &AWaveSpawner::StartNextWave,
        Delay,
        false
    );
}

void AWaveSpawner::UpdateWaveCountdown()
{
    WaveCountdownRemaining -= 1.0f;

    if (WaveCountdownRemaining <= 0.0f)
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_WaveCountdown);
        OnWaveCountdownChanged.Broadcast(0);
        return;
    }

    OnWaveCountdownChanged.Broadcast(
        FMath::CeilToInt(WaveCountdownRemaining)
    );
}

void AWaveSpawner::CheckForVictory()
{
    if (GetWorld() == nullptr)
    {
        return;
    }

    for (TActorIterator<ACoffinEndPoint> It(GetWorld()); It; ++It)
    {
        if (IsValid(*It) && It->IsGameOver())
        {
            GetWorldTimerManager().ClearTimer(TimerHandle_CheckVictory);
            return;
        }
    }

    for (TActorIterator<AEnemyBaseCpp> It(GetWorld()); It; ++It)
    {
        if (IsValid(*It))
        {
            return;
        }
    }

    GetWorldTimerManager().ClearTimer(TimerHandle_CheckVictory);

    OnAllWavesCompleted.Broadcast();
}

int32 AWaveSpawner::GetCurrentWave() const
{
    return FMath::Min(CurrentWaveIndex + 1, Waves.Num());
}

int32 AWaveSpawner::GetTotalWaves() const
{
    return Waves.Num();
}