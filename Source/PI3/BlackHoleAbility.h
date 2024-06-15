#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "BlackholeActor.h"
#include "BlackHoleAbility.generated.h"

UCLASS()
class PI3_API UBlackHoleAbility : public UBaseAbility
{
	GENERATED_BODY()

public:
	UBlackHoleAbility();

	virtual void Activate() override;
	void Deactivate();

	UFUNCTION(BlueprintCallable)
	void DestroyActiveBlackHole();

protected:
	UPROPERTY(EditAnywhere)
	float Range = 0.f;

	UPROPERTY(EditAnywhere)
	float SphereSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABlackholeActor> BlackHoleActorClass;

private:
	ABlackholeActor* ActiveBlackHole = nullptr;

	void ClearActiveBlackHole();

	bool bIsActivated = false;

};
