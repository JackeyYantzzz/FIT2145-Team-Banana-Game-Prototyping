#include "TowerSlot.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
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

    ClickBox->SetGenerateOverlapEvents(false);
}

void ATowerSlot::BeginPlay()
{
    Super::BeginPlay();

    if (ClickBox)
    {
        ClickBox->OnClicked.AddDynamic(
            this,
            &ATowerSlot::HandleClicked
        );
    }
}

void ATowerSlot::HandleClicked(
    UPrimitiveComponent* TouchedComponent,
    FKey ButtonPressed)
{
    if (ButtonPressed != EKeys::LeftMouseButton)
    {
        return;
    }

    if (bHasTower && !IsValid(SpawnedTower))
    {
        bHasTower = false;
        CurrentTowerCost = 0;
        SpawnedTower = nullptr;
        SetActorHiddenInGame(false);
    }

    if (bHasTower)
    {
        OnTowerSellSelected();
        return;
    }

    OnSlotSelected();
}

bool ATowerSlot::BuildTower(
    TSubclassOf<ATowerBase> TowerToBuild,
    int32 SoulCost)
{
    if (bHasTower)
    {
        return false;
    }

    if (TowerToBuild == nullptr || SoulCost <= 0)
    {
        return false;
    }

    UWorld* World = GetWorld();

    if (World == nullptr)
    {
        return false;
    }

    USoulWallet* SoulWallet = FindSoulWallet();

    if (SoulWallet == nullptr)
    {
        return false;
    }

    if (!SoulWallet->SpendSoul(SoulCost))
    {
        return false;
    }

    const FVector SpawnLocation =
        GetActorLocation() + TowerSpawnOffset;

    const FRotator SpawnRotation =
        FRotator::ZeroRotator;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    SpawnedTower = World->SpawnActor<ATowerBase>(
        TowerToBuild,
        SpawnLocation,
        SpawnRotation,
        SpawnParams
    );

    if (!IsValid(SpawnedTower))
    {
        SoulWallet->AddSoul(SoulCost);
        SpawnedTower = nullptr;
        bHasTower = false;
        CurrentTowerCost = 0;

        return false;
    }

    bHasTower = true;
    CurrentTowerCost = SoulCost;

    SetActorHiddenInGame(true);

    return true;
}

bool ATowerSlot::SellTower()
{
    if (!bHasTower)
    {
        return false;
    }

    if (!IsValid(SpawnedTower))
    {
        SpawnedTower = nullptr;
        bHasTower = false;
        CurrentTowerCost = 0;

        SetActorHiddenInGame(false);

        return false;
    }

    USoulWallet* SoulWallet = FindSoulWallet();

    if (SoulWallet == nullptr)
    {
        return false;
    }

    const int32 RefundAmount = CurrentTowerCost / 2;

    SoulWallet->AddSoul(RefundAmount);

    SpawnedTower->Destroy();
    SpawnedTower = nullptr;

    bHasTower = false;
    CurrentTowerCost = 0;

    SetActorHiddenInGame(false);

    return true;
}

int32 ATowerSlot::GetSellRefund() const
{
    return CurrentTowerCost / 2;
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
        if (USoulWallet* SoulWallet =
            ActorIt->FindComponentByClass<USoulWallet>())
        {
            return SoulWallet;
        }
    }

    return nullptr;
}