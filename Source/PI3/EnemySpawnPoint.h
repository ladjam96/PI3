#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnPoint.generated.h"

UCLASS()
class PI3_API AEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:
	AEnemySpawnPoint();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AEnemyCharacter> BasicEnemyClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ASpeedEnemy> FastEnemyClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ATankEnemy> BossEnemyClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnRadius;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float BasicEnemySpawnInterval;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float FastEnemySpawnInterval;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float BossEnemySpawnInterval;
	

private:
	void SpawnEnemy(TSubclassOf<ACharacter> EnemyClass);
	void SpawnBasicEnemy();
	void SpawnFastEnemy();
	void SpawnBossEnemy();


	FTimerHandle BasicEnemySpawnTimerHandle;
	FTimerHandle FastEnemySpawnTimerHandle;
	FTimerHandle BossEnemySpawnTimerHandle;
};
