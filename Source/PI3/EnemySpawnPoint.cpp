#include "EnemySpawnPoint.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "EnemyCharacter.h"
#include "SpeedEnemy.h"
#include "TankEnemy.h"

AEnemySpawnPoint::AEnemySpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RespawnTime = 10.0f;
}


void AEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnEnemy();
}

void AEnemySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawnPoint::SpawnEnemy()
{
	if (EnemyClass)
	{
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AEnemySpawnPoint::SpawnEnemy, RespawnTime, false);
	}
	if(EnemyClassSpeed)
	{
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<ASpeedEnemy>(EnemyClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AEnemySpawnPoint::SpawnEnemy, RespawnTime, false);
	}
	if(EnemyClassTank)
	{
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<ATankEnemy>(EnemyClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AEnemySpawnPoint::SpawnEnemy, RespawnTime, false);
	}
}


