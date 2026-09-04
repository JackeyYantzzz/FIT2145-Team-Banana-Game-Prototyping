#pragma once

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerSlot.generated.h"

class ATowerBase;
class UBoxComponent;
class USceneComponent;
class USoulWallet;

UCLASS()
class A1_API ATowerSlot : public AActor
{
	GENERATED_BODY()

public:
	ATowerSlot();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	bool bHasTower = false;

	UFUNCTION(BlueprintCallable, Category = "Tower Slot")
	bool BuildTower(TSubclassOf<ATowerBase> TowerToBuild, int32 SoulCost);

	UFUNCTION(BlueprintCallable, Category = "Tower Slot")
	bool SellTower();

	UFUNCTION(BlueprintPure, Category = "Tower Slot")
	int32 GetSellRefund() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	TObjectPtr<UBoxComponent> ClickBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	FVector TowerSpawnOffset = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	TObjectPtr<ATowerBase> SpawnedTower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	int32 CurrentTowerCost = 0;

	UFUNCTION(BlueprintImplementableEvent, Category = "Tower Slot")
	void OnSlotSelected();

	UFUNCTION(BlueprintImplementableEvent, Category = "Tower Slot")
	void OnTowerSellSelected();

	UFUNCTION()
	void HandleClicked(
		UPrimitiveComponent* TouchedComponent,
		FKey ButtonPressed
	);

private:
	USoulWallet* FindSoulWallet() const;
};