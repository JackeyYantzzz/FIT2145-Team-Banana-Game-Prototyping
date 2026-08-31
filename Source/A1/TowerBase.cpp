#include "TowerBase.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "EnemyBaseCpp.h"
#include "TowerProjectileBase.h"

ATowerBase::ATowerBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TowerSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TowerSprite"));
	TowerSprite->SetupAttachment(RootComponent);

	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeSphere"));
	AttackRangeSphere->SetupAttachment(RootComponent);
	AttackRangeSphere->SetSphereRadius(AttackRange);
	AttackRangeSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	AttackRangeSphere->SetGenerateOverlapEvents(true);
}

void ATowerBase::BeginPlay()
{
	Super::BeginPlay();

	AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ATowerBase::OnSphereBeginOverlap);
	AttackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ATowerBase::OnSphereEndOverlap);

	AttackRangeSphere->SetSphereRadius(AttackRange);

	if (AttackRate > 0.0f)
	{
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ATowerBase::AttackTarget, AttackRate, true);
	}
}

void ATowerBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (AEnemyBaseCpp* Enemy = Cast<AEnemyBaseCpp>(OtherActor))
	{
		TargetEnemies.AddUnique(Enemy);
	}
}

void ATowerBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AEnemyBaseCpp* Enemy = Cast<AEnemyBaseCpp>(OtherActor))
	{
		TargetEnemies.Remove(Enemy);
	}
}

void ATowerBase::AttackTarget()
{
	TargetEnemies.RemoveAll([](AEnemyBaseCpp* Enemy) {
		return !IsValid(Enemy) || Enemy->IsDead();
		});

	if (TargetEnemies.Num() == 0)
	{
		return;
	}

	AEnemyBaseCpp* TargetEnemy = TargetEnemies[0];
	if (!IsValid(TargetEnemy) || !ProjectileClass)
	{
		return;
	}

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = (TargetEnemy->GetActorLocation() - SpawnLocation).Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (ATowerProjectileBase* Projectile = GetWorld()->SpawnActor<ATowerProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams))
	{
		Projectile->InitializeProjectile(TargetEnemy, Damage);
	}
}