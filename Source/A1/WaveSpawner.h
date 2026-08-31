#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveSpawner.generated.h"

USTRUCT(BlueprintType)
struct FWaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 EnemyCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float SpawnInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float NextWaveDelay = 5.0f;
};

UCLASS()
class A1_API AWaveSpawner : public AActor
{
	GENERATED_BODY()

public:
	AWaveSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Setup")
	TArray<FWaveData> Waves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Setup")
	AActor* SplinePathRef;

private:
	int32 CurrentWaveIndex;
	int32 SpawnedInCurrentWave;

	FTimerHandle TimerHandle_SpawnEnemy;
	FTimerHandle TimerHandle_NextWave;

	void StartNextWave();
	void SpawnSingleEnemy();
	void OnWaveCompleted();
};