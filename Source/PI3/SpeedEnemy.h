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

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere)
	bool bIsDead = false;

	UPROPERTY(EditAnywhere)
	float AttackRange = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackCooldown = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackMontage = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathMontage = nullptr;

	UPROPERTY(EditAnywhere)
	float Damage = 0.f;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintPure)
	FVector GetCharacterVelocity() const;

	UFUNCTION(BlueprintPure)
	FRotator GetCharacterDirection() const;

	UFUNCTION()
	void ScaleStatsWithPlayerLevel(int32 PlayerLevel);

	UPROPERTY(EditAnywhere)
	float MaxHealth = 0.f;
	
	UPROPERTY(EditAnywhere)
	float CurrentHealth = 0.f;

	UPROPERTY()
	float XP = 0.f;

protected:
	virtual void BeginPlay() override;

	API3Character* TargetPlayer = nullptr;

	float LastAttackTime = 0.f;

	void Move();
	void AttackPlayer(float DamageAmount);
	void Die();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UWidgetComponent* HealthBarWidgetComponent = nullptr;

	UEnemyHealthBar* HealthBarWidget = nullptr;

};
