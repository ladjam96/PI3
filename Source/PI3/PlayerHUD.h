#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

UCLASS()
class PI3_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void UpdateHealthBar(float CurrentHealth, float MaxHealth);
	void UpdateHealthText(float CurrentHealth, float MaxHealth);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (BindWidget))
	class UTextBlock* HealthText;
};
