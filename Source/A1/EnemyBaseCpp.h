#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBaseCpp.generated.h"

class UCapsuleComponent;
class UPaperFlipbook;
class UPaperFlipbookComponent;
class USoulRewardComponent;

UCLASS()
class A1_API AEnemyBaseCpp : public AActor
{
	GENERATED_BODY()

public:
	AEnemyBaseCpp();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UPaperFlipbookComponent> EnemySprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soul")
	TObjectPtr<USoulRewardComponent> SoulReward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float MaxHP = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	float CurrentHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UPaperFlipbook> WalkFlipbook = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UPaperFlipbook> DeathFlipbook = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float DeathDestroyDelay = 0.6f;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	bool bIsDead = false;

public:
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void ReceiveDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void Die();

	UFUNCTION(BlueprintPure, Category = "Enemy")
	bool IsDead() const;

	UFUNCTION(BlueprintPure, Category = "Enemy")
	float GetHealthPercent() const;

private:
	UFUNCTION()
	void FinishDeath();
};
