#include "BlackHoleAbility.h"
#include "BlackholeActor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBlackHoleAbility::UBlackHoleAbility()
{
	AbilityName = "BlackHole Ability";
	Cooldown = 10.f;

	Range = 1500.f;
	SphereSpeed = 1000.f;
}

void UBlackHoleAbility::Activate()
{
	if (!IsOnCooldown() && !bIsActivated)
	{
		AActor* Owner = GetOwner();

		if (Owner && BlackHoleActorClass)
		{
			FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.f;

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Owner;
			SpawnParams.Instigator = Owner->GetInstigator();

			ActiveBlackHole = GetWorld()->SpawnActor<ABlackholeActor>(BlackHoleActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			if (ActiveBlackHole)
			{
				ActiveBlackHole->Initialize(SpawnLocation, Owner->GetActorForwardVector(), Range);
			}

			bIsActivated = true;
			ResetCooldown();
		}
	}
}

void UBlackHoleAbility::Deactivate()
{
	if (bIsActivated)
	{
		DestroyActiveBlackHole();
		bIsActivated = false;
	}
}

void UBlackHoleAbility::DestroyActiveBlackHole()
{
	if (ActiveBlackHole)
	{
		ActiveBlackHole->Destroy();
		ActiveBlackHole = nullptr;
	}
}

void UBlackHoleAbility::ClearActiveBlackHole()
{
	if (ActiveBlackHole)
	{
		ActiveBlackHole->Destroy();
		ActiveBlackHole = nullptr;
	}
}
