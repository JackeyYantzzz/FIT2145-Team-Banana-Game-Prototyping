#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBaseCpp.generated.h"

class UCapsuleComponent;
class UPaperFlipbook;
class UPaperFlipbookComponent;
class USoulRewardComponent;
class USplineComponent;

UCLASS()
class A1_API AEnemyBaseCpp : public AActor
{
	GENERATED_BODY()

public:
	AEnemyBaseCpp();

	virtual void Tick(float DeltaTime) override;

	// Damage interface called when hit by projectiles
	UFUNCTION(BlueprintCallable, Category = "Enemy|Combat")
	virtual void ApplyDamage(float Amount);

	// Slow debuff interface called by status-effect projectiles (e.g., Ice Tower)
	UFUNCTION(BlueprintCallable, Category = "Enemy|Combat")
	virtual void ApplySlow(float Multiplier, float Duration);

	// Assign target spline path for movement
	UFUNCTION(BlueprintCallable, Category = "Enemy|Movement")
	void SetTargetSpline(USplineComponent* InSpline);

	UFUNCTION(BlueprintPure, Category = "Enemy|Stats")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintPure, Category = "Enemy|Stats")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, Category = "Enemy|Stats")
	float GetCurrentMoveSpeed() const { return CurrentMoveSpeed; }

protected:
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPaperFlipbookComponent> EnemySprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USoulRewardComponent> SoulReward;

	// Core Stats (Configurable via Blueprint Defaults)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	float MaxHP = 35.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Stats")
	float CurrentHP = 35.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	float BaseMoveSpeed = 300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Stats")
	float CurrentMoveSpeed = 300.0f;

	// Visual Animations
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Visual")
	TObjectPtr<UPaperFlipbook> WalkFlipbook = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Visual")
	TObjectPtr<UPaperFlipbook> DeathFlipbook = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Stats")
	float DeathDestroyDelay = 0.6f;

	// Spline Movement Tracking
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Enemy|Movement")
	TObjectPtr<USplineComponent> TargetSpline = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Enemy|Movement")
	float DistanceAlongSpline = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Stats")
	bool bIsDead = false;

	FTimerHandle SlowTimerHandle;

	void UpdateSplineMovement(float DeltaTime);
	void ResetMoveSpeed();
	void Die();
	void OnReachedEnd();

private:
	UFUNCTION()
	void FinishDeath();
};