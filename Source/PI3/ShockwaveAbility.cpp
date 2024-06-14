#include "ShockwaveAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

UShockwaveAbility::UShockwaveAbility()
{
    AbilityName = "Shockwave";
    Cooldown = 5.f;
    NiagaraSystem = nullptr;
}

void UShockwaveAbility::Activate()
{
    if (!IsOnCooldown() && !bIsActivated)
    {
        AActor* Owner = GetOwner();
        if (Owner)
        {
            ActivationLocation = GetActivationLocation();
            ApplyShockwaveEffect(ActivationLocation);
            
            bIsActivated = true;

            ResetCooldown();
        }
    }
}

FVector UShockwaveAbility::GetActivationLocation() const
{
    AActor* Owner = GetOwner();
    if (Owner)
    {
        return Owner->GetActorLocation();
    }

    return FVector::ZeroVector;
}

void UShockwaveAbility::ApplyShockwaveEffect(const FVector& Origin)
{
    TArray<AActor*> OverlappingActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor != GetOwner())
        {
            FVector Direction = Actor->GetActorLocation() - Origin;
            Direction.Normalize();

            if (UPrimitiveComponent* OtherPrimitiveComponent = Actor->FindComponentByClass<UPrimitiveComponent>())
            {
                if (OtherPrimitiveComponent->IsSimulatingPhysics() && bApplyForceToPhysicsObjects)
                {
                    OtherPrimitiveComponent->AddImpulse(Direction * Force);
                }
                else if (ACharacter* Character = Cast<ACharacter>(Actor))
                {
                    if (bApplyForceToCharacters)
                    {
                        Character->LaunchCharacter(Direction * Force, true, true);
                    }
                }
            }
        }
    }

    if (NiagaraSystem)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, Origin, FRotator::ZeroRotator, FVector(1), true, true);
    }
}
