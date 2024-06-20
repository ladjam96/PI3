#include "BaseAttack.h"
#include "BasicAttackActor.h"
#include "Engine/World.h"

UBaseAttack::UBaseAttack()
{
    AbilityName = "Basic attack";
    Cooldown = 0.5f;
    
    Range = 2000.f;
    SphereSpeed = 1500.f;
    DamageAmount = 10.f;
}

void UBaseAttack::Activate()
{
    if (!IsOnCooldown() && !bIsActivated)
    {
        AActor* Owner = GetOwner();

        if (Owner && BasicAttackActorClass)
        {
            FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.f;

            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = Owner;
            SpawnParams.Instigator = Owner->GetInstigator();

            ActiveSphere = GetWorld()->SpawnActor<ABasicAttackActor>(BasicAttackActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
            if (ActiveSphere)
            {
                ActiveSphere->Initialize(SpawnLocation, Owner->GetActorForwardVector(), Range);
                ActiveSphere->SetBasicAbility(this);

                bIsActivated = true;
                ResetCooldown();
                
                float Duration = .5f;
                GetWorld()->GetTimerManager().SetTimer(SDeactivationTimerHandle, this, &UBaseAttack::Deactivate, Duration, false);
            }
        }
    }
}

void UBaseAttack::Deactivate()
{
    if (bIsActivated)
    {
        DestroyActiveSphere();
        bIsActivated = false;

        GetWorld()->GetTimerManager().ClearTimer(SDeactivationTimerHandle);
    }
}

void UBaseAttack::DestroyActiveSphere()
{
    if (ActiveSphere)
    {
        ActiveSphere->Destroy();
        ActiveSphere = nullptr;
    }
}

void UBaseAttack::HandleDeactivationTimer()
{
    Deactivate();
}
