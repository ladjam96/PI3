// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalGameMode.h"

#include "Kismet/GameplayStatics.h"

ASurvivalGameMode::ASurvivalGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyCount = 0;
	SpawnInterval = 5.0f;
	TimeSinceLastSpawn = 0.0f;
	SpawnRadius = 1000.0f;
}

void ASurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ASurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastSpawn += DeltaTime;
	if (TimeSinceLastSpawn >= SpawnInterval)
	{
		SpawnEnemies();
		TimeSinceLastSpawn = 0.0f;
	}
}

void ASurvivalGameMode::SpawnEnemies()
{
	FVector SpawnLocation = GetRandomSpawnLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;

	//GetWorld()->SpawnActor<AEnemyCharacter>(AEnemyCharacter::StaticClass(), SpawnLocation, SpawnRotation);

	EnemyCount++;
}

FVector ASurvivalGameMode::GetRandomSpawnLocation()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		return FVector::ZeroVector;
	}

	FVector PlayerLocation = PlayerPawn->GetActorLocation();

	FVector RandomLocation;
	float Angle = FMath::RandRange(0.0f, 360.0f);
	RandomLocation.X = PlayerLocation.X + SpawnRadius * FMath::Cos(FMath::DegreesToRadians(Angle));
	RandomLocation.Y = PlayerLocation.Y + SpawnRadius * FMath::Sin(FMath::DegreesToRadians(Angle));
	RandomLocation.Z = PlayerLocation.Z;

	return RandomLocation;
}