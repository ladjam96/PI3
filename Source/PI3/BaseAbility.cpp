#include "BaseAbility.h"

UBaseAbility::UBaseAbility()
{
	PrimaryComponentTick.bCanEverTick = true;

	Cooldown = 0.f;
	CurrentCooldown = 0.f;
}

void UBaseAbility::BeginPlay()
{
	Super::BeginPlay();	
}

void UBaseAbility::Activate()
{
	
}

bool UBaseAbility::IsOnCooldown() const
{
	return CurrentCooldown > 0;
}

void UBaseAbility::UpdateCooldown(float DeltaTime)
{
	if(CurrentCooldown > 0)
	{
		CurrentCooldown -= DeltaTime;
		if(CurrentCooldown < 0)
		{
			CurrentCooldown = 0;
		}
	}
}

void UBaseAbility::ResetCooldown()
{
	CurrentCooldown = Cooldown;
}
