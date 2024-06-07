#include "MainMenu.h"

#include "PI3Character.h"
#include "Components/Button.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (UButton* NewGameButtonPtr = Cast<UButton>(GetWidgetFromName(TEXT("NewGameButton"))))
	{
		NewGameButtonPtr->OnClicked.AddDynamic(this, &UMainMenu::OnNewGameClicked);
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

void UMainMenu::OnExitClicked()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
}
