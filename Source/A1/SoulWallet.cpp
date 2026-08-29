#include "SoulWallet.h"

USoulWallet::USoulWallet()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USoulWallet::BeginPlay()
{
    Super::BeginPlay();

    CurrentSoul = FMath::Max(0, InitialSoul);
    OnSoulChanged.Broadcast(CurrentSoul);
}

int32 USoulWallet::GetSoul() const
{
    return CurrentSoul;
}

bool USoulWallet::HasEnoughSoul(int32 Amount) const
{
    return Amount > 0 && CurrentSoul >= Amount;
}

void USoulWallet::AddSoul(int32 Amount)
{
    if (Amount <= 0)
    {
        return;
    }

    CurrentSoul += Amount;
    OnSoulChanged.Broadcast(CurrentSoul);
}

bool USoulWallet::SpendSoul(int32 Amount)
{
    if (!HasEnoughSoul(Amount))
    {
        return false;
    }

    CurrentSoul -= Amount;
    OnSoulChanged.Broadcast(CurrentSoul);
    return true;
}

void USoulWallet::SetSoul(int32 NewAmount)
{
    CurrentSoul = FMath::Max(0, NewAmount);
    OnSoulChanged.Broadcast(CurrentSoul);
}
