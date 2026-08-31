#include "WaveSpawner.h"
#include "Engine/World.h"
#include "TimerManager.h"

AWaveSpawner::AWaveSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	CurrentWaveIndex = 0;
	SpawnedInCurrentWave = 0;
	SplinePathRef = nullptr;
}

void AWaveSpawner::BeginPlay()
{
	Super::BeginPlay();
	StartNextWave();
}

void AWaveSpawner::StartNextWave()
{
	if (!Waves.IsValidIndex(CurrentWaveIndex))
	{
		return;
	}

	SpawnedInCurrentWave = 0;
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

	if (SpawnedInCurrentWave >= CurrentWave.EnemyCount)
	{
		OnWaveCompleted();
		return;
	}

	if (CurrentWave.EnemyClass && GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform SpawnTransform = GetActorTransform();
		GetWorld()->SpawnActor<AActor>(CurrentWave.EnemyClass, SpawnTransform, SpawnParams);

		SpawnedInCurrentWave++;
	}
}

void AWaveSpawner::OnWaveCompleted()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnEnemy);

	const float Delay = Waves[CurrentWaveIndex].NextWaveDelay;
	CurrentWaveIndex++;

	GetWorldTimerManager().SetTimer(
		TimerHandle_NextWave,
		this,
		&AWaveSpawner::StartNextWave,
		Delay,
		false
	);
}