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

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemyCharacter> BasicEnemyClass = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASpeedEnemy> FastEnemyClass = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ATankEnemy> BossEnemyClass = nullptr;

	UPROPERTY(EditAnywhere)
	float SpawnRadius = 0.f;

	UPROPERTY(EditAnywhere)
	float BasicEnemySpawnInterval = 0.f;

	UPROPERTY(EditAnywhere)
	float FastEnemySpawnInterval = 0.f;

	UPROPERTY(EditAnywhere)
	float BossEnemySpawnInterval = 0.f;

protected:
	virtual void BeginPlay() override;

private:
	void SpawnEnemy(TSubclassOf<ACharacter> EnemyClass);
	
	void SpawnBasicEnemy();
	
	void SpawnFastEnemy();
	
	void SpawnBossEnemy();
	
	FTimerHandle BasicEnemySpawnTimerHandle;
	FTimerHandle FastEnemySpawnTimerHandle;
	FTimerHandle BossEnemySpawnTimerHandle;
};
