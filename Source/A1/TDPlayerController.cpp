#include "TDPlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

ATDPlayerController::ATDPlayerController()
{
	// Enable mouse cursor and click events
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	// Default grid cell size
	GridSize = 100.0f;
}

void ATDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	OccupiedGridCells.Empty();
}

void ATDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &ATDPlayerController::OnLeftClick);
	}
}

FVector ATDPlayerController::SnapToGrid(const FVector& WorldLocation, FIntPoint& OutGridCoord) const
{
	if (GridSize <= 0.0f) return WorldLocation;

	// Calculate grid column and row index
	int32 GridX = FMath::RoundToInt(WorldLocation.X / GridSize);
	int32 GridY = FMath::RoundToInt(WorldLocation.Y / GridSize);

	OutGridCoord = FIntPoint(GridX, GridY);

	// Return the center position of the grid cell (maintaining the original Z height)
	return FVector(GridX * GridSize, GridY * GridSize, WorldLocation.Z);
}

void ATDPlayerController::OnLeftClick()
{
	FHitResult HitResult;
	// Cast a ray from cursor into the scene
	if (GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		AActor* HitActor = HitResult.GetActor();
		if (!HitActor || AvailableTowers.Num() == 0 || !GetWorld()) return;

		// Prevent building on roads or spline paths
		if (HitActor->ActorHasTag(FName("Road")) || HitActor->GetName().Contains(TEXT("Spline")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot build towers on the road!"));
			return;
		}

		// Calculate snapped grid location and grid coordinate
		FIntPoint GridCoord;
		FVector SnappedLocation = SnapToGrid(HitResult.Location, GridCoord);

		// Check if the current grid cell is already occupied
		if (OccupiedGridCells.Contains(GridCoord))
		{
			UE_LOG(LogTemp, Warning, TEXT("Grid cell already occupied!"));
			return;
		}

		// Pick a random tower class among the 3 options
		int32 RandomIndex = FMath::RandRange(0, AvailableTowers.Num() - 1);
		TSubclassOf<AActor> SelectedTowerClass = AvailableTowers[RandomIndex];

		if (SelectedTowerClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Spawn tower at snapped location
			FTransform SpawnTransform(FRotator::ZeroRotator, SnappedLocation);
			AActor* SpawnedTower = GetWorld()->SpawnActor<AActor>(SelectedTowerClass, SpawnTransform, SpawnParams);

			if (SpawnedTower)
			{
				// Mark the cell as occupied
				OccupiedGridCells.Add(GridCoord);
			}
		}
	}
}