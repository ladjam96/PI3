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
	class UProgressBar* HealthBar = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* HealthText = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ExpBar = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ExpText = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* LvlText = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* BlackholeBar = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ShockwaveBar = nullptr;
	
	//Pause Menu
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PauseButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> BP_PauseMenu = nullptr;

	UFUNCTION()
	void OnPauseClick();

	virtual void NativeConstruct() override;
};
