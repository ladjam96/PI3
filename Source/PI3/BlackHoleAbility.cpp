#include "BlackHoleAbility.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBlackHoleAbility::UBlackHoleAbility()
{
	AbilityName = "BlackHole Ability";
	Cooldown = 10.f;
	Range = 500.f;
	SlowDownFactor = 0.5f;
}

void UBlackHoleAbility::Activate()
{
	if(!IsOnCooldown())
	{
		AActor* Owner = GetOwner();
		
		if(Owner)
		{
			FVector CharacterLocation = Owner->GetActorLocation();
			TArray<AActor*> OverlappingActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), OverlappingActors);

			for (AActor* Actor : OverlappingActors)
			{
				if (FVector::Dist(CharacterLocation, Actor->GetActorLocation()) <= Range)
				{
					FVector Direction = (CharacterLocation - Actor->GetActorLocation()).GetSafeNormal();
					// Apply attraction and slowdown effect to the enemy
					// Implement actual movement and slowdown logic here
				}
			}

			ResetCooldown();
		}
	}
}
