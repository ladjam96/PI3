// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SurvivalGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PI3_API ASurvivalGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
ASurvivalGameMode();

protected:
virtual void BeginPlay() override;

public:
virtual void Tick(float DeltaTime) override;

private:
void SpawnEnemies();
FVector GetRandomSpawnLocation();

int32 EnemyCount;
float SpawnInterval;
float TimeSinceLastSpawn;

UPROPERTY(EditAnywhere, Category = "Spawning")
float SpawnRadius;
};
