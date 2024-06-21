#include "MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (UButton* NewGameButtonPtr = Cast<UButton>(GetWidgetFromName(TEXT("NewGameButton"))))
	{
		NewGameButtonPtr->OnClicked.AddDynamic(this, &UMainMenu::OnNewGameClicked);
	}

	if (UButton* ControlsButtonPtr = Cast<UButton>(GetWidgetFromName(TEXT("ControlsButton"))))
	{
		ControlsButtonPtr->OnClicked.AddDynamic(this, &UMainMenu::OnControlsClicked);
	}

	if (UButton* ExitButtonPtr = Cast<UButton>(GetWidgetFromName(TEXT("ExitButton"))))
	{
		ExitButtonPtr->OnClicked.AddDynamic(this, &UMainMenu::OnExitClicked);
	}
}

void UMainMenu::OnNewGameClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("ThirdPersonMap"), true);
}

void UMainMenu::OnControlsClicked()
{
	if (!ControlsOverlayClass) return;
	
	if (!ControlsOverlay)
	{
		ControlsOverlay = CreateWidget<UUserWidget>(GetWorld(), ControlsOverlayClass);
	}

	if (ControlsOverlay)
	{
		ControlsOverlay->AddToViewport();
		
		if (UButton* CloseButton = Cast<UButton>(ControlsOverlay->GetWidgetFromName(TEXT("CloseButton"))))
		{
			CloseButton->OnClicked.AddDynamic(this, &UMainMenu::OnCloseControlsClicked);
		}
	}
}

void UMainMenu::OnCloseControlsClicked()
{
	if (ControlsOverlay)
	{
		ControlsOverlay->RemoveFromViewport();
	}
}

void UMainMenu::OnExitClicked()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
}
