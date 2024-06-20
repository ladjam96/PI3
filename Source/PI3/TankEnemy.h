#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PI3Character.h"
#include "EnemyHealthBar.h"
#include "Components/WidgetComponent.h"
#include "TankEnemy.generated.h"

UCLASS()
class PI3_API ATankEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ATankEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Combat
	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bIsAttacking;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bIsDead;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintPure)
	FVector GetCharacterVelocity() const;

	UFUNCTION(BlueprintPure)
	FRotator GetCharacterDirection() const;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxHealth;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float CurrentHealth;

protected:
	API3Character* TargetPlayer;

	float LastAttackTime;

	void Move();
	void AttackPlayer(float DamageAmount);
	void Die();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	class UWidgetComponent* HealthBarWidgetComponent;

	UEnemyHealthBar* HealthBarWidget;
};
