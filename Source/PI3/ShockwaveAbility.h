#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "ShockwaveAbility.generated.h"

UCLASS()
class PI3_API UShockwaveAbility : public UBaseAbility
{
	GENERATED_BODY()

	UShockwaveAbility();

	virtual void Activate() override;

protected:
	UPROPERTY(EditAnywhere)
	float Force = 0.f;
};
