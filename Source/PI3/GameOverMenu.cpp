#include "GameOverMenu.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (UButton* NewGameButtonPtr = Cast<UButton>(GetWidgetFromName(TEXT("ContinueButton"))))
	{
		NewGameButtonPtr->OnClicked.AddDynamic(this, &UGameOverMenu::OnContinueClicked);
	}

	if (UButton* ExitButtonPtr = Cast<UButton>(GetWidgetFromName(TEXT("ExitButton"))))
	{
		ExitButtonPtr->OnClicked.AddDynamic(this, &UGameOverMenu::OnExitClicked);
	}}

void UGameOverMenu::OnContinueClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("ThirdPersonMap"), true);
}

void UGameOverMenu::OnExitClicked()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
}
