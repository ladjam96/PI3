// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include"Kismet/GameplayStatics.h"
#include"NavigationSystem.h"
#include"EnemyCharacter.h"
#include"Components/SphereComponent.h"
#include"Components/BoxComponent.h"
#include"PI3Character.h"
#include"GameFramework/CharacterMovementComponent.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	bSearchForPlayer = true;
	bCanAttackPlayer = true;
	bMoveToPlayer = false; //puede ser trueee mirar

	GenerateRandomSearchLocation();
	SearchForPlayer();

	
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEnemyAIController::GenerateRandomSearchLocation()
{
	//RamdonLocation = NavArea ->GetRandomReachablePointInRadius(this, GetPawn()->GetActorLocation(),10000.0f);
}

void AEnemyAIController::SearchForPlayer()
{
}

void AEnemyAIController::MoveToPlayer()
{
}

void AEnemyAIController::StartChasingPlayer()
{
}

bool AEnemyAIController::IsEnemyCloseToPlayer()
{
	return false;
}

void AEnemyAIController::AttackPlayer()
{
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
}

void AEnemyAIController::OnDetectPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AEnemyAIController::OnDetectPlayerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
