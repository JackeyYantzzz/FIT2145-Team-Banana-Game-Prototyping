#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoffinEndPoint.generated.h"

class AEnemyBaseCpp;
class UBoxComponent;
class UPaperSpriteComponent;
class USceneComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoffinHealthChanged, int32, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCoffinDestroyed);

UCLASS()
class A1_API ACoffinEndPoint : public AActor
{
	GENERATED_BODY()

public:
	ACoffinEndPoint();

	UFUNCTION(BlueprintPure, Category = "Coffin")
	int32 GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Coffin")
	void ResetCoffin();

	UFUNCTION(BlueprintCallable, Category = "Coffin")
	void RegisterEnemyHit(AActor* EnemyActor);

	UFUNCTION(BlueprintPure, Category = "Coffin")
	bool IsGameOver() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> EndTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPaperSpriteComponent> CoffinSprite;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coffin")
	int32 MaxHealth = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coffin")
	int32 CurrentHealth = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coffin")
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coffin")
	FName EnemyTag = TEXT("Enemy");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coffin")
	bool bDestroyEnemyOnHit = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coffin")
	bool bPrintDebugMessages = true;

	UPROPERTY(BlueprintAssignable, Category = "Coffin")
	FCoffinHealthChanged OnCoffinHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Coffin")
	FCoffinDestroyed OnCoffinDestroyed;

	UFUNCTION(BlueprintImplementableEvent, Category = "Coffin")
	void HandleGameOver();

private:
	UFUNCTION()
	void HandleTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	bool IsEnemyActor(AActor* Actor) const;
};
