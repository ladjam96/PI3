#include "ShockwaveAbility.h"

#include "EnemyCharacter.h"
#include "SpeedEnemy.h"
#include "TankEnemy.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

UShockwaveAbility::UShockwaveAbility()
{
    AbilityName = "Shockwave";
    Cooldown = 5.f;
    Force = 100000.0f;
    Radius = 600.0f;

    DamageAmount = 20.f;
    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShockwaveNiagaraComponent"));
    NiagaraComponent->SetAutoActivate(false);
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
            if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(Actor))
            {
                EnemyCharacter->TakeDamage(DamageAmount);
            }
            else if (ASpeedEnemy* SpeedEnemy = Cast<ASpeedEnemy>(Actor))
            {
                SpeedEnemy->TakeDamage(DamageAmount);
            }
            else if (ATankEnemy* TankEnemy = Cast<ATankEnemy>(Actor))
            {
                TankEnemy->TakeDamage(DamageAmount);
            }

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

    if (NiagaraComponent)
    {
        NiagaraComponent->SetWorldLocation(Origin);
        NiagaraComponent->Activate(true);

        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UShockwaveAbility::DeactivateNiagaraComponent, NiagaraDuration, false);
    }
}

void UShockwaveAbility::DeactivateNiagaraComponent()
{
    if (NiagaraComponent)
    {
        NiagaraComponent->Deactivate();
    }
    bIsActivated = false;
}
