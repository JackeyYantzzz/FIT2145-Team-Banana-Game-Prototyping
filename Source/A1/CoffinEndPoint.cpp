#include "CoffinEndPoint.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "EnemyBaseCpp.h"
#include "Engine/Engine.h"
#include "PaperSpriteComponent.h"

ACoffinEndPoint::ACoffinEndPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	CoffinSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("CoffinSprite"));
	CoffinSprite->SetupAttachment(Root);
	CoffinSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EndTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EndTrigger"));
	EndTrigger->SetupAttachment(Root);
	EndTrigger->SetBoxExtent(FVector(120.0f, 120.0f, 120.0f));
	EndTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EndTrigger->SetCollisionObjectType(ECC_WorldDynamic);
	EndTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	EndTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	EndTrigger->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	EndTrigger->SetGenerateOverlapEvents(true);

	EnemyClass = AEnemyBaseCpp::StaticClass();
}

void ACoffinEndPoint::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = FMath::Max(1, MaxHealth);
	OnCoffinHealthChanged.Broadcast(CurrentHealth);

	EndTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACoffinEndPoint::HandleTriggerBeginOverlap);
}

int32 ACoffinEndPoint::GetCurrentHealth() const
{
	return CurrentHealth;
}

void ACoffinEndPoint::ResetCoffin()
{
	CurrentHealth = FMath::Max(1, MaxHealth);
	OnCoffinHealthChanged.Broadcast(CurrentHealth);
}

void ACoffinEndPoint::RegisterEnemyHit(AActor* EnemyActor)
{
	if (!IsEnemyActor(EnemyActor) || CurrentHealth <= 0)
	{
		return;
	}

	CurrentHealth = FMath::Max(0, CurrentHealth - 1);
	OnCoffinHealthChanged.Broadcast(CurrentHealth);

	if (bPrintDebugMessages && GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Cyan,
			FString::Printf(TEXT("Coffin Health: %d"), CurrentHealth));
	}

	if (bDestroyEnemyOnHit && IsValid(EnemyActor))
	{
		EnemyActor->Destroy();
	}

	if (CurrentHealth <= 0)
	{
		if (bPrintDebugMessages && GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, TEXT("Game Over"));
		}

		OnCoffinDestroyed.Broadcast();
		HandleGameOver();
	}
}

bool ACoffinEndPoint::IsGameOver() const
{
	return CurrentHealth <= 0;
}

void ACoffinEndPoint::HandleTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!IsEnemyActor(OtherActor))
	{
		return;
	}

	RegisterEnemyHit(OtherActor);
}

bool ACoffinEndPoint::IsEnemyActor(AActor* Actor) const
{
	if (!IsValid(Actor) || Actor == this)
	{
		return false;
	}

	if (EnemyClass != nullptr)
	{
		return Actor->IsA(EnemyClass);
	}

	return Actor->IsA(AEnemyBaseCpp::StaticClass()) || Actor->ActorHasTag(EnemyTag);
}
