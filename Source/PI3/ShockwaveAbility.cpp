#include "ShockwaveAbility.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UShockwaveAbility::UShockwaveAbility()
{
	AbilityName = "Shockwave";
	Cooldown = 5.f;
	Force = 1000.0f;
}

void UShockwaveAbility::Activate()
{
	if (!IsOnCooldown())
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			FVector CharacterLocation = Owner->GetActorLocation();
			TArray<AActor*> OverlappingActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), OverlappingActors);

			for (AActor* Actor : OverlappingActors)
			{
				if (Actor != Owner)
				{
					FVector Direction = (Actor->GetActorLocation() - CharacterLocation).GetSafeNormal();
					// Apply force to push enemies away
					// Implement actual push logic here
				}
			}

			ResetCooldown();
			UE_LOG(LogTemp, Warning, TEXT("ShockwaveAbility activated. Cooldown reset to: %f"), Cooldown);
		}
	}
}
