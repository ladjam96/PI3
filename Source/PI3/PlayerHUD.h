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

	void UpdateExpBar(float CurrentExperience, float ExperienceToNextLevel);
	void UpdateExpText(float CurrentExperience, float ExperienceToNextLevel);

	void UpdateLevelText(int32 CurrentLevel);

	void UpdateBlackholeBar(float Progress);
	void UpdateShockwaveBar(float Progress);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ExpBar;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ExpText;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* LvlText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* BlackholeBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ShockwaveBar;
	
	//Pause Menu
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PauseButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> BP_PauseMenu;

	UFUNCTION()
	void OnPauseClick();

	virtual void NativeConstruct() override;
};
