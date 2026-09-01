#include "MainMenuGameMode.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	PlayerControllerClass = APlayerController::StaticClass();
	DefaultPawnClass = nullptr;
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuGameMode could not find a player controller."));
		return;
	}

	if (!MainMenuClass)
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuClass is not configured on BP_MainMenuGameMode."));
		return;
	}

	MainMenuWidget = CreateWidget<UUserWidget>(PlayerController, MainMenuClass);
	if (!MainMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuGameMode failed to create the main menu widget."));
		return;
	}

	MainMenuWidget->AddToViewport();

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;
}
