#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "NiagaraSystem.h"
#include "ShockwaveAbility.generated.h"

class UShockwaveComponent;

UCLASS()
class PI3_API UShockwaveAbility : public UBaseAbility
{
    GENERATED_BODY()

public:
    UShockwaveAbility();

    virtual void Activate() override;

protected:
    UPROPERTY(EditAnywhere)
    float Force = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Radius = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UNiagaraSystem* NiagaraSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bApplyForceToPhysicsObjects = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bApplyForceToCharacters = true;

private:
    FVector GetActivationLocation() const;

    void ApplyShockwaveEffect(const FVector& Origin);

    FVector ActivationLocation;
    bool bIsActivated = false;
};
