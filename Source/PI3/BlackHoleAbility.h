#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "BlackHoleAbility.generated.h"

UCLASS()
class PI3_API UBlackHoleAbility : public UBaseAbility
{
	GENERATED_BODY()

public:
	
	UBlackHoleAbility();
	
	virtual void Activate() override;

protected:
	UPROPERTY(EditAnywhere)
	float Range = 0.f;

	UPROPERTY(EditAnywhere)
	float SlowDownFactor = 0.f;
};
