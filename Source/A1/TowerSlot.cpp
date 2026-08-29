#include "TowerSlot.h"

#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "InputCoreTypes.h"
#include "SoulWallet.h"
#include "TowerBase.h"

ATowerSlot::ATowerSlot()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ClickBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ClickBox"));
	ClickBox->SetupAttachment(Root);
	ClickBox->SetBoxExtent(FVector(64.0f, 64.0f, 32.0f));
	ClickBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ClickBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	ClickBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void ATowerSlot::BeginPlay()
{
	Super::BeginPlay();

	ClickBox->OnClicked.AddDynamic(this, &ATowerSlot::HandleClicked);
}

void ATowerSlot::HandleClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (bHasTower || TowerClass == nullptr)
	{
		return;
	}

	USoulWallet* SoulWallet = FindSoulWallet();
	if (SoulWallet == nullptr || !SoulWallet->SpendSoul(SoulCost))
	{
		return;
	}

	const FVector SpawnLocation = GetActorLocation() + TowerSpawnOffset;
	const FRotator SpawnRotation = GetActorRotation();

	SpawnedTower = GetWorld()->SpawnActor<ATowerBase>(TowerClass, SpawnLocation, SpawnRotation);
	bHasTower = SpawnedTower != nullptr;
}

USoulWallet* ATowerSlot::FindSoulWallet() const
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return nullptr;
	}

	for (TActorIterator<AActor> ActorIt(World); ActorIt; ++ActorIt)
	{
		if (USoulWallet* SoulWallet = ActorIt->FindComponentByClass<USoulWallet>())
		{
			return SoulWallet;
		}
	}

	return nullptr;
}
