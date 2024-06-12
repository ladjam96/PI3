// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PI3_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

public:
	APawn* PlayerPawn;

	class AEnemyCharacter;

	class UNavigationSystemV1* NavArea;
	FVector RamdonLocation = FVector();

	bool bMoveToPlayer;
	bool bSearchForPlayer;
	bool bCanAttackPlayer;

	float PlayerStoppingDistance = 60.0f;
	float PlayerAttackDistance = 200.0f;

	void GenerateRandomSearchLocation();
	void SearchForPlayer();

	void MoveToPlayer();
	void StartChasingPlayer();
	bool IsEnemyCloseToPlayer();
	void AttackPlayer();

	FTimerHandle SearchTimerHandle;
	FTimerHandle AttackTimerHandle;

public:

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION()
		void OnDetectPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnDetectPlayerEndOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	
};
