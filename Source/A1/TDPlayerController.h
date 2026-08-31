#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDPlayerController.generated.h"

UCLASS()
class A1_API ATDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATDPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// The 3 candidate tower classes (configure BP_Tower_1, 2, 3 in Blueprint Details)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Setup")
	TArray<TSubclassOf<AActor>> AvailableTowers;

	// Size of a single grid cell in Unreal units (e.g., 50.0f, 64.0f, or 100.0f)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Setup")
	float GridSize;

	// Tracks occupied grid coordinates to prevent placing multiple towers on the same cell
	UPROPERTY()
	TSet<FIntPoint> OccupiedGridCells;

	void OnLeftClick();

	// Snaps world coordinates to the nearest grid center
	FVector SnapToGrid(const FVector& WorldLocation, FIntPoint& OutGridCoord) const;
};