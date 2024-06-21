#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

UCLASS()
class PI3_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ContinueButton = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* RestartButton = nullptr;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnContinueClick();
	
	UFUNCTION()
	void OnRestartClick();
};
