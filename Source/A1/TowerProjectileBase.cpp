#include "TowerProjectileBase.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "EnemyBaseCpp.h"
#include "Kismet/KismetSystemLibrary.h"

ATowerProjectileBase::ATowerProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetCollisionProfileName(TEXT("NoCollision"));

	ProjectileSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ProjectileSprite"));
	ProjectileSprite->SetupAttachment(RootComponent);
}

void ATowerProjectileBase::InitializeProjectile(AEnemyBaseCpp* InTarget, float InDamage)
{
	TargetEnemy = InTarget;
	Damage = InDamage;
}

void ATowerProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(TargetEnemy) || TargetEnemy->IsDead())
	{
		Destroy();
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = TargetEnemy->GetActorLocation();

	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	FVector NewLocation = CurrentLocation + Direction * MoveSpeed * DeltaTime;
	SetActorLocation(NewLocation);

	if (FVector::Dist(NewLocation, TargetLocation) <= 30.0f)
	{
		OnHitTarget();
	}
}

void ATowerProjectileBase::OnHitTarget()
{
	if (bIsAOEProjectile)
	{
		ApplyAOEDamage();
	}
	else
	{
		if (IsValid(TargetEnemy))
		{
			TargetEnemy->ApplyDamage(Damage);

			if (bIsSlowProjectile)
			{
				TargetEnemy->ApplySlow(SlowMultiplier, SlowDuration);
			}
		}
	}

	Destroy();
}

void ATowerProjectileBase::ApplyAOEDamage()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	TArray<AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		AOERadius,
		ObjectTypes,
		AEnemyBaseCpp::StaticClass(),
		IgnoredActors,
		OutActors
	);

	for (AActor* Actor : OutActors)
	{
		if (AEnemyBaseCpp* Enemy = Cast<AEnemyBaseCpp>(Actor))
		{
			Enemy->ApplyDamage(Damage);
		}
	}
}