#include "SoulDropVisual.h"

#include "PaperFlipbookComponent.h"
#include "TimerManager.h"

ASoulDropVisual::ASoulDropVisual()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("SoulFlipbook"));
	FlipbookComponent->SetupAttachment(Root);
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASoulDropVisual::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	SetActorScale3D(FVector(StartScale));

	if (SoulFlipbook != nullptr)
	{
		FlipbookComponent->SetFlipbook(SoulFlipbook);
		FlipbookComponent->PlayFromStart();
	}

	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ASoulDropVisual::FinishDrop, LifeSeconds, false);
}

void ASoulDropVisual::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AgeSeconds += DeltaSeconds;
	const float Progress = LifeSeconds > 0.0f ? FMath::Clamp(AgeSeconds / LifeSeconds, 0.0f, 1.0f) : 1.0f;

	SetActorLocation(FMath::Lerp(StartLocation, StartLocation + FloatOffset, Progress));
	SetActorScale3D(FVector(FMath::Lerp(StartScale, EndScale, Progress)));
}

void ASoulDropVisual::FinishDrop()
{
	Destroy();
}
