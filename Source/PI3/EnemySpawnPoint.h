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
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemyCharacter> BasicEnemyClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASpeedEnemy> FastEnemyClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ATankEnemy> BossEnemyClass;

	UPROPERTY(EditAnywhere)
	float SpawnRadius;

	UPROPERTY(EditAnywhere)
	float BasicEnemySpawnInterval;

	UPROPERTY(EditAnywhere)
	float FastEnemySpawnInterval;

	UPROPERTY(EditAnywhere)
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
