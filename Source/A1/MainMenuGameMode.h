#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

class UUserWidget;

UCLASS()
class A1_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainMenuGameMode();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main Menu")
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> MainMenuWidget;
};
