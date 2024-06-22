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

	UPROPERTY(EditAnywhere)
	float Range = 0.f;

	UPROPERTY(EditAnywhere)
	float SphereSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABlackholeActor> BlackHoleActorClass = nullptr;

	UPROPERTY(EditAnywhere)
	USoundBase* BlackholeSound = nullptr;

private:
	ABlackholeActor* ActiveBlackHole = nullptr;
	
	bool bIsActivated = false;

	FTimerHandle DeactivationTimerHandle;

	void HandleDeactivationTimer();

};