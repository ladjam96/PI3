#include "EnemySpawnPoint.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "EnemyCharacter.h"
#include "SpeedEnemy.h"
#include "TankEnemy.h"
#include "Kismet/GameplayStatics.h"

AEnemySpawnPoint::AEnemySpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpawnRadius = 1000.0f;
	BasicEnemySpawnInterval = 5.0f;
	FastEnemySpawnInterval = 10.0f;
	BossEnemySpawnInterval = 60.0f;
}


void AEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(BasicEnemySpawnTimerHandle, this, &AEnemySpawnPoint::SpawnBasicEnemy, BasicEnemySpawnInterval, true);
	GetWorldTimerManager().SetTimer(FastEnemySpawnTimerHandle, this, &AEnemySpawnPoint::SpawnFastEnemy, FastEnemySpawnInterval, true);
	GetWorldTimerManager().SetTimer(BossEnemySpawnTimerHandle, this, &AEnemySpawnPoint::SpawnBossEnemy, BossEnemySpawnInterval, true);
}

void AEnemySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawnPoint::SpawnBasicEnemy()
{
	SpawnEnemy(BasicEnemyClass);
}

void AEnemySpawnPoint::SpawnFastEnemy()
{
	SpawnEnemy(FastEnemyClass);
}

void AEnemySpawnPoint::SpawnBossEnemy()
{
	SpawnEnemy(BossEnemyClass);
}

void AEnemySpawnPoint::SpawnEnemy(TSubclassOf<ACharacter> EnemyClass)
{
	if (EnemyClass == nullptr) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn == nullptr) return;

	FVector SpawnLocation;
	bool bValidSpawnLocation = false;
	int32 MaxAttempts = 10;
	int32 Attempts = 0;

	while (!bValidSpawnLocation && Attempts < MaxAttempts)
	{
		FVector2D RandPointInCircle = FMath::RandPointInCircle(SpawnRadius);
		SpawnLocation = PlayerPawn->GetActorLocation() + FVector(RandPointInCircle, 0.0f); // Convertir FVector2D a FVector
		Attempts++;
		
		if (FVector::Dist(SpawnLocation, PlayerPawn->GetActorLocation()) > 900.0f) // Asegúrate de que los enemigos no se generen demasiado cerca del jugador
		{
			bValidSpawnLocation = true;
		}
	}

	if (bValidSpawnLocation)
	{
		ACharacter* SpawnedEnemy = GetWorld()->SpawnActor<ACharacter>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
		if (SpawnedEnemy)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawned enemy of class: %s at location: %s"), *EnemyClass->GetName(), *SpawnLocation.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn enemy of class: %s"), *EnemyClass->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find a valid spawn location"));
	}
}


