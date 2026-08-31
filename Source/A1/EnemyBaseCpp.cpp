#include "EnemyBaseCpp.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "PaperFlipbookComponent.h"
#include "SoulRewardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AEnemyBaseCpp::AEnemyBaseCpp()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	Capsule->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Capsule->SetGenerateOverlapEvents(true);

	EnemySprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemySprite"));
	EnemySprite->SetupAttachment(Capsule);
	EnemySprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SoulReward = CreateDefaultSubobject<USoulRewardComponent>(TEXT("SoulReward"));
}

void AEnemyBaseCpp::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = FMath::Max(1.0f, MaxHP);
	CurrentMoveSpeed = BaseMoveSpeed;
	DistanceAlongSpline = 0.0f;

	if (WalkFlipbook != nullptr)
	{
		EnemySprite->SetFlipbook(WalkFlipbook);
		EnemySprite->PlayFromStart();
	}

	// Auto-find Spline Actor in level if not set
	if (!TargetSpline)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("EnemyPath"), FoundActors);

		if (FoundActors.Num() > 0)
		{
			TargetSpline = FoundActors[0]->FindComponentByClass<USplineComponent>();
		}
		else
		{
			// Fallback: search by class name containing Spline
			TArray<AActor*> AllActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
			for (AActor* Actor : AllActors)
			{
				if (IsValid(Actor) && Actor->GetClass()->GetName().Contains(TEXT("Spline")))
				{
					TargetSpline = Actor->FindComponentByClass<USplineComponent>();
					if (TargetSpline)
					{
						break;
					}
				}
			}
		}
	}
}

void AEnemyBaseCpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsDead)
	{
		UpdateSplineMovement(DeltaTime);
	}
}

void AEnemyBaseCpp::UpdateSplineMovement(float DeltaTime)
{
	if (!TargetSpline)
	{
		return;
	}

	DistanceAlongSpline += CurrentMoveSpeed * DeltaTime;
	const float SplineLength = TargetSpline->GetSplineLength();

	if (DistanceAlongSpline >= SplineLength)
	{
		OnReachedEnd();
		return;
	}

	const FVector NewLocation = TargetSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	const FRotator NewRotation = TargetSpline->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void AEnemyBaseCpp::ApplyDamage(float Amount)
{
	if (bIsDead || Amount <= 0.0f)
	{
		return;
	}

	CurrentHP = FMath::Max(0.0f, CurrentHP - Amount);

	if (CurrentHP <= 0.0f)
	{
		Die();
	}
}

void AEnemyBaseCpp::ApplySlow(float Multiplier, float Duration)
{
	if (bIsDead)
	{
		return;
	}

	CurrentMoveSpeed = BaseMoveSpeed * FMath::Clamp(Multiplier, 0.1f, 1.0f);

	GetWorldTimerManager().ClearTimer(SlowTimerHandle);
	GetWorldTimerManager().SetTimer(SlowTimerHandle, this, &AEnemyBaseCpp::ResetMoveSpeed, Duration, false);
}

void AEnemyBaseCpp::ResetMoveSpeed()
{
	CurrentMoveSpeed = BaseMoveSpeed;
}

void AEnemyBaseCpp::SetTargetSpline(USplineComponent* InSpline)
{
	TargetSpline = InSpline;
	DistanceAlongSpline = 0.0f;
}

void AEnemyBaseCpp::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	GetWorldTimerManager().ClearTimer(SlowTimerHandle);
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

void AEnemyBaseCpp::OnReachedEnd()
{
	// Enemy reached base / end of path
	Destroy();
}

void AEnemyBaseCpp::FinishDeath()
{
	Destroy();
}

float AEnemyBaseCpp::GetHealthPercent() const
{
	if (MaxHP <= 0.0f)
	{
		return 0.0f;
	}

	return CurrentHP / MaxHP;
}