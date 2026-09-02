#include "WaveSpawner.h"
#include "Engine/World.h"
#include "TimerManager.h"

AWaveSpawner::AWaveSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	CurrentWaveIndex = 0;
	CurrentSubWaveIndex = 0;
	SpawnedInCurrentSubWave = 0;
	SplinePathRef = nullptr;
	WaveCountdownRemaining = 0.0f;
}

void AWaveSpawner::BeginPlay()
{
	Super::BeginPlay();
	StartNextWave();
}

void AWaveSpawner::StartNextWave()
{
	// Reset countdown state
	GetWorldTimerManager().ClearTimer(TimerHandle_WaveCountdown);
	OnWaveCountdownChanged.Broadcast(0);

	if (!Waves.IsValidIndex(CurrentWaveIndex))
	{
		return;
	}

	// Reset sub-wave tracking counters
	CurrentSubWaveIndex = 0;
	SpawnedInCurrentSubWave = 0;

	// Notify UI of wave progress
	OnWaveChanged.Broadcast(
		CurrentWaveIndex + 1,
		Waves.Num()
	);

	const FWaveData& CurrentWave = Waves[CurrentWaveIndex];

	// Start repeating timer for individual spawns
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

	// Iterate through sub-waves to find the active group with remaining count
	while (CurrentSubWaveIndex < CurrentWave.SubWaves.Num())
	{
		const FEnemySubWave& SubWave = CurrentWave.SubWaves[CurrentSubWaveIndex];

		if (SpawnedInCurrentSubWave < SubWave.EnemyCount)
		{
			// Spawn enemy actor at spawner location
			if (SubWave.EnemyClass && GetWorld())
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride =
					ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				const FTransform SpawnTransform = GetActorTransform();

				GetWorld()->SpawnActor<AActor>(
					SubWave.EnemyClass,
					SpawnTransform,
					SpawnParams
				);
			}

			SpawnedInCurrentSubWave++;
			return; // Spawn one actor per timer tick
		}

		// Current enemy type completed, move to next sub-wave group
		CurrentSubWaveIndex++;
		SpawnedInCurrentSubWave = 0;
	}

	// All sub-waves in this wave completed
	OnWaveCompleted();
}

void AWaveSpawner::OnWaveCompleted()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnEnemy);

	const float Delay = Waves[CurrentWaveIndex].NextWaveDelay;

	CurrentWaveIndex++;

	// All waves finished
	if (!Waves.IsValidIndex(CurrentWaveIndex))
	{
		OnWaveCountdownChanged.Broadcast(0);
		return;
	}

	// Initialize and broadcast intermission countdown
	WaveCountdownRemaining = Delay;

	OnWaveCountdownChanged.Broadcast(
		FMath::CeilToInt(WaveCountdownRemaining)
	);

	// Start countdown tick timer (every 1.0 second)
	GetWorldTimerManager().SetTimer(
		TimerHandle_WaveCountdown,
		this,
		&AWaveSpawner::UpdateWaveCountdown,
		1.0f,
		true
	);

	// Schedule the start of the next wave
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

int32 AWaveSpawner::GetCurrentWave() const
{
	return CurrentWaveIndex + 1;
}

int32 AWaveSpawner::GetTotalWaves() const
{
	return Waves.Num();
}