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

	// Root scene component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Clickable collision box
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

	// Bind mouse click event to the box component
	if (ClickBox)
	{
		ClickBox->OnClicked.AddDynamic(this, &ATowerSlot::HandleClicked);
	}
}

void ATowerSlot::HandleClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	// Only accept left mouse click and ensure slot is vacant
	if (ButtonPressed != EKeys::LeftMouseButton || bHasTower)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("TowerSlot [%s] clicked! Opening UI selection menu."), *GetName());

	// Trigger Blueprint to open WBP_TowerSelectMenu
	OnSlotSelected();
}

bool ATowerSlot::BuildTower(TSubclassOf<ATowerBase> TowerToBuild, int32 SoulCost)
{
	UE_LOG(LogTemp, Warning, TEXT("=====> BuildTower Called on [%s] with Cost: %d <====="), *GetName(), SoulCost);

	// Check if already occupied
	if (bHasTower)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildTower Failed: Slot already has a tower built!"));
		return false;
	}

	// Validate tower class
	if (TowerToBuild == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildTower Failed: TowerToBuild class parameter is NULL!"));
		return false;
	}

	// Validate world
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildTower Failed: World is null!"));
		return false;
	}

	// Search for SoulWallet
	USoulWallet* SoulWallet = FindSoulWallet();
	if (SoulWallet == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildTower Failed: No USoulWallet found in the world! Make sure BP_TDPlayerController is equipped."));
		return false;
	}

	// Spend souls
	if (!SoulWallet->SpendSoul(SoulCost))
	{
		UE_LOG(LogTemp, Error, TEXT("BuildTower Failed: Not enough souls! Current: %d, Required: %d"),
			SoulWallet->GetSoul(), SoulCost);
		return false;
	}

	// Calculate transform (lock rotation to zero to prevent unwanted tilting)
	const FVector SpawnLocation = GetActorLocation() + TowerSpawnOffset;
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn tower actor
	SpawnedTower = World->SpawnActor<ATowerBase>(TowerToBuild, SpawnLocation, SpawnRotation, SpawnParams);
	bHasTower = (SpawnedTower != nullptr);

	if (bHasTower)
	{
		UE_LOG(LogTemp, Log, TEXT("BuildTower SUCCESS: Tower [%s] successfully spawned at %s! Remaining Souls: %d"),
			*SpawnedTower->GetName(), *SpawnLocation.ToString(), SoulWallet->GetSoul());

		// Disable slot collision so it does not interfere with the tower or block traces
		if (ClickBox)
		{
			ClickBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		// Hide the entire slot actor (visual mesh / indicator plane) once the tower is built
		SetActorHiddenInGame(true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BuildTower Failed: World->SpawnActor returned nullptr!"));
	}

	return bHasTower;
}

USoulWallet* ATowerSlot::FindSoulWallet() const
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return nullptr;
	}

	// Iterate through all actors to find the SoulWallet component
	for (TActorIterator<AActor> ActorIt(World); ActorIt; ++ActorIt)
	{
		if (USoulWallet* SoulWallet = ActorIt->FindComponentByClass<USoulWallet>())
		{
			return SoulWallet;
		}
	}

	return nullptr;
}