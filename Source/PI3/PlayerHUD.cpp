#include "PlayerHUD.h"

#include "CustomPlayerController.h"
#include "PI3Character.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

//Pause Game
void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
	if(PauseButton)
	{
		PauseButton->OnClicked.AddDynamic(this, &UPlayerHUD::OnPauseClick);
	}

	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(UGameplayStatics::GetPlayerController(GetWorld(), 0), this);
}

//Health Bar
void UPlayerHUD::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	ACustomPlayerController* PC = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
	API3Character* Character = Cast<API3Character>(PC->GetCharacter());
	
	if (HealthBar)
	{
		if (MaxHealth > 0)
		{
			HealthBar->SetPercent(Character->CurrentHealth / Character->MaxHealth);
		}
		else
		{
			HealthBar->SetPercent(0.0f);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HealthBar reference is nullptr in PlayerHUD!"));
	}
}

void UPlayerHUD::UpdateHealthText(float CurrentHealth, float MaxHealth)
{
	ACustomPlayerController* PC = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
	API3Character* Character = Cast<API3Character>(PC->GetCharacter());
	
	if (HealthText)
	{
		FString HealthString = FString::Printf(TEXT("%d / %d"), FMath::RoundToInt(Character->CurrentHealth), FMath::RoundToInt(Character->MaxHealth));
		HealthText->SetText(FText::FromString(HealthString));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HealthText reference is nullptr in PlayerHUD!"));
	}
}

void UPlayerHUD::UpdateExpBar(float CurrentExperience, float ExperienceToNextLevel)
{
	if (ExpBar)
	{
		ExpBar->SetPercent(CurrentExperience / ExperienceToNextLevel);
	}
}

void UPlayerHUD::UpdateExpText(float CurrentExperience, float ExperienceToNextLevel)
{
	if (ExpText)
	{
		ExpText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentExperience, ExperienceToNextLevel)));
	}
}

void UPlayerHUD::UpdateLevelText(int32 CurrentLevel)
{
	if (LvlText)
	{
		LvlText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevel)));
	}
}
//Pause Game
void UPlayerHUD::OnPauseClick()
{
	if(IsValid(BP_PauseMenu))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		
		UUserWidget* Widget = CreateWidget(GetWorld(), BP_PauseMenu);

		if(Widget)
		{
			Widget->AddToViewport();
		}
	}
}
