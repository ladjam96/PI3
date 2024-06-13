// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PI3Character.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class PI3_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AEnemyCharacter();
	

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;
	//MOVE//
	UFUNCTION(BlueprintPure)
	FVector GetCharacterVelocity() const;
	UFUNCTION(BlueprintPure)
	FRotator GetCharacterDirection() const;
	UFUNCTION()
	void Move();
	UPROPERTY(BlueprintReadOnly)
	FVector2D MovementVector;
	UPROPERTY(BlueprintReadOnly)
	FVector2D RotationVector;
	//
	
	//Attack//
	UFUNCTION()
	void AttackPlayer();
	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bIsAttacking;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	UAnimMontage* AttackMontage;
	//

	//Widget//
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> HealthBarWidgetClass;
	UPROPERTY()
	class UWidgetComponent* HealthBarWidgetComponent;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float CurrentHealth;
	//UFUNCTION(BlueprintCallable)
	//void UpdateHealthBar(float CurrentHealth, float MaxHealth);
	
private:
	
	API3Character* TargetPlayer;
	float LastAttackTime;
};