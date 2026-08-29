#include "EnemyBaseCpp.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "SoulRewardComponent.h"
#include "TimerManager.h"

AEnemyBaseCpp::AEnemyBaseCpp()
{
	PrimaryActorTick.bCanEverTick = false;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);

	EnemySprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemySprite"));
	EnemySprite->SetupAttachment(Capsule);
	EnemySprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SoulReward = CreateDefaultSubobject<USoulRewardComponent>(TEXT("SoulReward"));
}

void AEnemyBaseCpp::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = FMath::Max(1.0f, MaxHP);

	if (WalkFlipbook != nullptr)
	{
		EnemySprite->SetFlipbook(WalkFlipbook);
		EnemySprite->PlayFromStart();
	}
}

void AEnemyBaseCpp::ReceiveDamage(float DamageAmount)
{
	if (bIsDead || DamageAmount <= 0.0f)
	{
		return;
	}

	CurrentHP = FMath::Max(0.0f, CurrentHP - DamageAmount);

	if (CurrentHP <= 0.0f)
	{
		Die();
	}
}

void AEnemyBaseCpp::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (SoulReward != nullptr)
	{
		SoulReward->GrantSoulReward();
	}

	if (DeathFlipbook != nullptr)
	{
		EnemySprite->SetFlipbook(DeathFlipbook);
		EnemySprite->PlayFromStart();
	}

	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemyBaseCpp::FinishDeath, DeathDestroyDelay, false);
}

bool AEnemyBaseCpp::IsDead() const
{
	return bIsDead;
}

float AEnemyBaseCpp::GetHealthPercent() const
{
	if (MaxHP <= 0.0f)
	{
		return 0.0f;
	}

	return CurrentHP / MaxHP;
}

void AEnemyBaseCpp::FinishDeath()
{
	Destroy();
}
