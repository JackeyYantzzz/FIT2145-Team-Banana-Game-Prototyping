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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	TObjectPtr<UBoxComponent> ClickBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	TSubclassOf<ATowerBase> TowerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	int32 SoulCost = 25;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	FVector TowerSpawnOffset = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	bool bHasTower = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Slot")
	TObjectPtr<ATowerBase> SpawnedTower;

	UFUNCTION()
	void HandleClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

private:
	USoulWallet* FindSoulWallet() const;
};
