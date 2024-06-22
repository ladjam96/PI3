#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "WorldGeneration.generated.h"

UCLASS()
class PI3_API AWorldGeneration : public AActor
{
	GENERATED_BODY()
    
public:    
	AWorldGeneration();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int XVertexCount = 50;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int YVertexCount = 50;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CellSize = 1000;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumOfSectionX = 2;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumOfSectionY = 2;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MeshSectionIndex = 0;
    
	UPROPERTY(BlueprintReadOnly)
	UProceduralMeshComponent* TerrainMesh;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* TerrainMaterial = nullptr;

protected:
	virtual void BeginPlay() override;

public:    
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void GenerateTerrain(const int SectionIndexX, const int SectionIndexY);
    
	float GetHeight(FVector2D Location); // if we want at the end to add height to the map level

private:
	FVector2D LastPlayerSection;
	void CheckAndGenerateNewTerrain();
	TWeakObjectPtr<AActor> Player; 
	void FindPlayer();
};
