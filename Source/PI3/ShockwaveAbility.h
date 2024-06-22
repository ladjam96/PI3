#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "NiagaraComponent.h"
#include "ShockwaveAbility.generated.h"

UCLASS()
class PI3_API UShockwaveAbility : public UBaseAbility
{
    GENERATED_BODY()

public:
    UShockwaveAbility();

    virtual void Activate() override;

    UPROPERTY(EditAnywhere)
    float Force = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Radius = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UNiagaraComponent* NiagaraComponent = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bApplyForceToPhysicsObjects = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bApplyForceToCharacters = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float NiagaraDuration = 0.4f;

    UPROPERTY(EditAnywhere)
    USoundBase* ShockwaveSound = nullptr;

private:
    FVector GetActivationLocation() const;

    void ApplyShockwaveEffect(const FVector& Origin);

    void DeactivateNiagaraComponent();

    FVector ActivationLocation {0};
    bool bIsActivated = false;
};
