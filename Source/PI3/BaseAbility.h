#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAbility.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PI3_API UBaseAbility : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseAbility();

	virtual void Activate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentCooldown = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount = 0.f;

	UFUNCTION()
	float GetDamageAmount() { return DamageAmount ;}
	
	UFUNCTION(BlueprintCallable)
	bool IsOnCooldown() const;

	UFUNCTION(BlueprintCallable)
	void UpdateCooldown(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void ResetCooldown();

protected:
	virtual void BeginPlay() override;
		
};
