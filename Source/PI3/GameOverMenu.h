#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverMenu.generated.h"

UCLASS()
class PI3_API UGameOverMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnContinueClicked();

	UFUNCTION()
	void OnExitClicked();

	UPROPERTY(EditAnywhere)
	USoundBase* ClickSound = nullptr;
};
