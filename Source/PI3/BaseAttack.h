#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "BaseAttack.generated.h"

class ABasicAttackActor;

UCLASS()
class PI3_API UBaseAttack : public UBaseAbility
{
	GENERATED_BODY()

public:
	UBaseAttack();

	virtual void Activate() override;
	void Deactivate();

	UFUNCTION(BlueprintCallable)
	void DestroyActiveSphere();

	UPROPERTY(EditAnywhere)
	float Range = 0.f;

	UPROPERTY(EditAnywhere)
	float SphereSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABasicAttackActor> BasicAttackActorClass;

private:
	ABasicAttackActor* ActiveSphere = nullptr;

	bool bIsActivated = false;

	FTimerHandle SDeactivationTimerHandle;

	void HandleDeactivationTimer();
};
