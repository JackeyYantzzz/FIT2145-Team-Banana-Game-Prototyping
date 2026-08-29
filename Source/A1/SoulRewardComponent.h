#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoulRewardComponent.generated.h"

class ASoulDropVisual;
class USoulWallet;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class A1_API USoulRewardComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoulRewardComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soul Reward")
	int32 SoulReward = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soul Reward")
	TSubclassOf<ASoulDropVisual> SoulDropClass;

	UFUNCTION(BlueprintCallable, Category = "Soul Reward")
	void GrantSoulReward();

private:
	USoulWallet* FindSoulWallet() const;
};
