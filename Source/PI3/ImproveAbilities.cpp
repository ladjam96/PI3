#include "ImproveAbilities.h"
#include "BlackHoleAbility.h"
#include "ShockwaveAbility.h"
#include "BaseAttack.h"
#include "PI3Character.h"
#include "Components/Button.h"

void UImproveAbilities::InitializeAbilities(UBlackHoleAbility* InBlackHoleAbility, UShockwaveAbility* InShockwaveAbility, UBaseAttack* InBaseAttack)
{
	BlackHoleAbility = InBlackHoleAbility;
	ShockwaveAbility = InShockwaveAbility;
	BaseAttack = InBaseAttack;

	if (ImproveBlackhole)
	{
		ImproveBlackhole->OnClicked.AddDynamic(this, &UImproveAbilities::OnImproveBlackholeClick);
	}

	if (ImproveShockwave)
	{
		ImproveShockwave->OnClicked.AddDynamic(this, &UImproveAbilities::OnImproveShockwaveClick);
	}

	if (ImproveBasicAttack)
	{
		ImproveBasicAttack->OnClicked.AddDynamic(this, &UImproveAbilities::OnImproveBasicAttackClick);
	}
}

void UImproveAbilities::SetOwningCharacter(API3Character* OwningChar)
{
	OwningCharacter = OwningChar;
}

void UImproveAbilities::OnImproveBlackholeClick()
{
	if (BlackHoleAbility)
	{
		BlackHoleAbility->DamageAmount *= 1.15f;
		BlackHoleAbility->Range *= 1.15f;
		BlackHoleAbility->SphereSpeed *= 1.15f;
	}

	HideMenu();
}

void UImproveAbilities::OnImproveShockwaveClick()
{
	if (ShockwaveAbility)
	{
		ShockwaveAbility->DamageAmount *= 1.15f;
		ShockwaveAbility->Radius *= 1.15f;
	}

	HideMenu();
}

void UImproveAbilities::OnImproveBasicAttackClick()
{
	if (BaseAttack)
	{
		BaseAttack->DamageAmount *= 1.15f;
		BaseAttack->Range *= 1.15f;
		BaseAttack->SphereSpeed *= 1.15f;
	}
	HideMenu();
}

void UImproveAbilities::HideMenu()
{
	RemoveFromParent();

	APlayerController* PlayerController = Cast<APlayerController>(OwningCharacter->GetController());

	if (PlayerController)
	{
		FInputModeGameAndUI InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetPause(false);
	}
}
