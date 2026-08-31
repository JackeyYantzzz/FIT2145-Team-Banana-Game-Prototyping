#include "TDPlayerController.h"
#include "SoulWallet.h"

ATDPlayerController::ATDPlayerController()
{
	
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;

	
	SoulWallet = CreateDefaultSubobject<USoulWallet>(TEXT("SoulWallet"));
}

void ATDPlayerController::BeginPlay()
{
	Super::BeginPlay();
}
