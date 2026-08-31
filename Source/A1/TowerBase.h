#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerBase.generated.h"

class USphereComponent;
class UPaperSpriteComponent;
class AEnemyBaseCpp;
class ATowerProjectileBase;

UCLASS()
class A1_API ATowerBase : public AActor
{
	GENERATED_BODY()

public:
	ATowerBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPaperSpriteComponent> TowerSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> AttackRangeSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Stats")
	float AttackRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Stats")
	float AttackRange = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Stats")
	float Damage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Combat")
	TSubclassOf<ATowerProjectileBase> ProjectileClass;

	UPROPERTY(VisibleInstanceOnly, Category = "Tower|Combat")
	TArray<TObjectPtr<AEnemyBaseCpp>> TargetEnemies;

	FTimerHandle AttackTimerHandle;

	void AttackTarget();

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};