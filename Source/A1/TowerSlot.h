#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerSlot.generated.h"

class ATowerBase;
class UBoxComponent;
class USoulWallet;

UCLASS()
class A1_API ATowerSlot : public AActor
{
	GENERATED_BODY()

public:
	ATowerSlot();

	// Whether a tower has already been built on this slot
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	bool bHasTower = false;

	// Callable by the UI selection buttons to build a chosen tower and deduct souls
	UFUNCTION(BlueprintCallable, Category = "Tower Slot")
	bool BuildTower(TSubclassOf<ATowerBase> TowerToBuild, int32 SoulCost);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	TObjectPtr<UBoxComponent> ClickBox;

	// Optional location offset when spawning the tower
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	FVector TowerSpawnOffset = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	TObjectPtr<ATowerBase> SpawnedTower;

	// Blueprint implementable event to spawn and display WBP_TowerSelectMenu
	UFUNCTION(BlueprintImplementableEvent, Category = "Tower Slot")
	void OnSlotSelected();

	// Click event callback on the collision box
	UFUNCTION()
	void HandleClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

private:
	// Helper function to locate the player's SoulWallet in the world
	USoulWallet* FindSoulWallet() const;
};