// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PI3Character.h"
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

	class USphereComponent* GetDetectPlayerCollisionSphere();
	
private:
	API3Character* TargetPlayer;

	//UPROPERTY(EditAnywhere)
		//class USphereComponent* DetectPlayerCollisionSphere();
};