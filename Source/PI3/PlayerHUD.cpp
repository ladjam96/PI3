#include "PlayerHUD.h"

#include "CustomPlayerController.h"
#include "PI3Character.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

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
