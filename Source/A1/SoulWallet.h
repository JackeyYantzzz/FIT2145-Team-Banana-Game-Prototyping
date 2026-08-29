#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoulWallet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoulChanged, int32, NewSoulAmount);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class A1_API USoulWallet : public UActorComponent
{
    GENERATED_BODY()

public:
    USoulWallet();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soul")
    int32 InitialSoul = 100;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soul")
    int32 CurrentSoul = 0;

    UPROPERTY(BlueprintAssignable, Category = "Soul")
    FOnSoulChanged OnSoulChanged;

    UFUNCTION(BlueprintCallable, Category = "Soul")
    int32 GetSoul() const;

    UFUNCTION(BlueprintCallable, Category = "Soul")
    bool HasEnoughSoul(int32 Amount) const;

    UFUNCTION(BlueprintCallable, Category = "Soul")
    void AddSoul(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Soul")
    bool SpendSoul(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Soul")
    void SetSoul(int32 NewAmount);
};