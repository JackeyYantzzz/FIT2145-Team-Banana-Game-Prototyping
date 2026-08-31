#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerProjectileBase.generated.h"

class USphereComponent;
class UPaperSpriteComponent;
class AEnemyBaseCpp;

UCLASS()
class A1_API ATowerProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	ATowerProjectileBase();

	virtual void Tick(float DeltaTime) override;

	void InitializeProjectile(AEnemyBaseCpp* InTarget, float InDamage);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPaperSpriteComponent> ProjectileSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Movement")
	float MoveSpeed = 800.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|WaterTower")
	bool bIsSlowProjectile = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|WaterTower", meta = (EditCondition = "bIsSlowProjectile"))
	float SlowMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|WaterTower", meta = (EditCondition = "bIsSlowProjectile"))
	float SlowDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|LightningTower")
	bool bIsAOEProjectile = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|LightningTower", meta = (EditCondition = "bIsAOEProjectile"))
	float AOERadius = 300.0f;

	UPROPERTY()
	TObjectPtr<AEnemyBaseCpp> TargetEnemy = nullptr;

	float Damage = 0.0f;

	void OnHitTarget();
	void ApplyAOEDamage();
};