#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

UCLASS()
class PI3_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnControlsClicked();

	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnCloseControlsClicked();

private:
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UUserWidget> ControlsOverlayClass;

	UPROPERTY()
	UUserWidget* ControlsOverlay;
};
