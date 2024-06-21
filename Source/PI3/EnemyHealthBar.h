#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "EnemyHealthBar.generated.h"

UCLASS()
class PI3_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthBar = nullptr;
};
