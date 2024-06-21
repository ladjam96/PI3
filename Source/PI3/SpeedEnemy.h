#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PI3Character.h"
#include "EnemyHealthBar.h"
#include "Components/WidgetComponent.h"
#include "SpeedEnemy.generated.h"

UCLASS()
class PI3_API ASpeedEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ASpeedEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Combat
	UPROPERTY(EditAnywhere)
	bool bIsAttacking;

	UPROPERTY(EditAnywhere)
	bool bIsDead;

	UPROPERTY(EditAnywhere)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere)
	float Damage;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintPure)
	FVector GetCharacterVelocity() const;

	UFUNCTION(BlueprintPure)
	FRotator GetCharacterDirection() const;
	void ScaleStatsWithPlayerLevel(int32 PlayerLevel);

	UPROPERTY(EditAnywhere)
	float MaxHealth;
	UPROPERTY(EditAnywhere)
	float CurrentHealth;

	UPROPERTY()
	float XP = 0.f;

protected:
	API3Character* TargetPlayer;

	float LastAttackTime;

	void Move();
	void AttackPlayer(float DamageAmount);
	void Die();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UWidgetComponent* HealthBarWidgetComponent;

	UEnemyHealthBar* HealthBarWidget;

};
