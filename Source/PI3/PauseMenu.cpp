#include "PauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if(ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UPauseMenu::OnContinueClick);
	}
	
	if(RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UPauseMenu::OnRestartClick);
	}
}

void UPauseMenu::OnContinueClick()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	RemoveFromParent();
}

void UPauseMenu::OnRestartClick()
{
	UWorld* World = GetWorld();
	
	if(World)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("RestartLevel"));
	}
}
