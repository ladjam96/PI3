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
	TSubclassOf<class AEnemyCharacter> EnemyClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ASpeedEnemy> EnemyClassSpeed;
	
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ATankEnemy> EnemyClassTank;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float RespawnTime;

	void SpawnEnemy();

private:
	FTimerHandle RespawnTimerHandle;
};
