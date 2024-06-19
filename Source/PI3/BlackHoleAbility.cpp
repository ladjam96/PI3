#include "BlackHoleAbility.h"
#include "BlackholeActor.h"

UBlackHoleAbility::UBlackHoleAbility()
{
	AbilityName = "BlackHole Ability";
	Cooldown = 10.f;

	Range = 1500.f;
	SphereSpeed = 1000.f;
	DamageAmount = 40.f;
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

				// Apply damage to actors within the black hole's range
				TArray<AActor*> OverlappingActors;
				ActiveBlackHole->GetOverlappingActors(OverlappingActors, AActor::StaticClass());

				for (AActor* Actor : OverlappingActors)
				{
					if (Actor != Owner)
					{
						if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actor))
						{
							Enemy->TakeDamage(DamageAmount);
						}
					}
				}
			}

			bIsActivated = true;
			ResetCooldown();

			float Duration = 5.0f;
			GetWorld()->GetTimerManager().SetTimer(DeactivationTimerHandle, this, &UBlackHoleAbility::Deactivate, Duration, false);
		}
	}
}



void UBlackHoleAbility::Deactivate()
{
	if (bIsActivated)
	{
		DestroyActiveBlackHole();
		bIsActivated = false;

		GetWorld()->GetTimerManager().ClearTimer(DeactivationTimerHandle);
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

void UBlackHoleAbility::HandleDeactivationTimer()
{
	Deactivate();
}
