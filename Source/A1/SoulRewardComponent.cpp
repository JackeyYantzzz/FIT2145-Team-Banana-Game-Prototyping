#include "SoulRewardComponent.h"

#include "Engine/World.h"
#include "EngineUtils.h"
#include "SoulDropVisual.h"
#include "SoulWallet.h"

USoulRewardComponent::USoulRewardComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USoulRewardComponent::GrantSoulReward()
{
	if (USoulWallet* SoulWallet = FindSoulWallet())
	{
		SoulWallet->AddSoul(SoulReward);
	}

	if (SoulDropClass != nullptr && GetOwner() != nullptr)
	{
		GetWorld()->SpawnActor<ASoulDropVisual>(
			SoulDropClass,
			GetOwner()->GetActorLocation(),
			GetOwner()->GetActorRotation());
	}
}

USoulWallet* USoulRewardComponent::FindSoulWallet() const
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
