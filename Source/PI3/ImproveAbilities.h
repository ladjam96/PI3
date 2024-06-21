#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ImproveAbilities.generated.h"

class UBaseAttack;
class UShockwaveAbility;
class UBlackHoleAbility;

UCLASS()
class PI3_API UImproveAbilities : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ImproveBlackhole = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ImproveShockwave = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ImproveBasicAttack = nullptr;

	UFUNCTION(BlueprintCallable)
	void InitializeAbilities(UBlackHoleAbility* InBlackHoleAbility, UShockwaveAbility* InShockwaveAbility, UBaseAttack* InBaseAttack);
	
	UFUNCTION()
	void OnImproveBlackholeClick();

	UFUNCTION()
	void OnImproveShockwaveClick();

	UFUNCTION()
	void OnImproveBasicAttackClick();

	UFUNCTION()
	void HideMenu();

	UFUNCTION()
	void SetOwningCharacter(API3Character* OwningChar);

private:
	UBlackHoleAbility* BlackHoleAbility = nullptr;
	UShockwaveAbility* ShockwaveAbility = nullptr;
	UBaseAttack* BaseAttack = nullptr;
	API3Character* OwningCharacter = nullptr;
};
