#include "WorldGeneration.h"
#include "KismetProceduralMeshLibrary.h"

AWorldGeneration::AWorldGeneration()
{
	PrimaryActorTick.bCanEverTick = true;

	TerrainMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainMesh"));
	TerrainMesh->SetupAttachment(GetRootComponent()); 
}

void AWorldGeneration::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWorldGeneration::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWorldGeneration::GenerateTerrain(const int SectionIndexX, const int SectionIndexY)
{
	FVector Offset = FVector(SectionIndexX*(XVertexCount-1), SectionIndexY*(YVertexCount-1), 0.f)*CellSize;

	TArray<FVector> Vertices;
	FVector Vertex;

	TArray<FVector2D> UVs;
	FVector2D UV;

	TArray<int32> Triangles;

	TArray<FVector> Normals;
	
	TArray<FProcMeshTangent> Tangents;

	//Vertices and UVs
	for(int32 iVY = -1; iVY <= YVertexCount; iVY++)
	{
		for(int32 iVX = -1; iVX <= XVertexCount; iVX++)
		{
			Vertex.X = iVX * CellSize + Offset.X;
			Vertex.Y = iVY * CellSize + Offset.Y;
			Vertex.Z = 0.f;
			Vertices.Add(Vertex);

			UV.X = (iVX + (SectionIndexX * (XVertexCount - 1))) * CellSize / 100; 
			UV.X = (iVY + (SectionIndexX * (XVertexCount - 1))) * CellSize / 100;  //Revisa esto tonto
			UVs.Add(UV);
		}
	}
	
	//Triangles
	for(int32 iTY = 0;  iTY <= YVertexCount; iTY++)
	{
		for(int32 iTX = 0;  iTX <= YVertexCount; iTX++)
		{
			Triangles.Add(iTX + iTY * (XVertexCount + 2));
			Triangles.Add(iTX + (iTY + 1 ) * (XVertexCount + 2));
			Triangles.Add(iTX + iTY * (XVertexCount + 2) + 1);

			Triangles.Add(iTX + (iTY + 1) * (XVertexCount + 2));
			Triangles.Add(iTX + (iTY +1) * (XVertexCount + 2) + 1);
			Triangles.Add(iTX + iTY * (XVertexCount + 2) + 1);
		}
	}
	//Subset Mesh
	TArray<FVector> SubVertices;
	
	TArray<FVector2D> SubUVs;
	
	TArray<int32> SubTriangles;

	TArray<FVector> SubNormals;
	
	TArray<FProcMeshTangent> SubTangents;

	int VertexIndex = 0;

	//Calculate Normals
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);

	//Subset vertices and UVs
	for(int32 iVY = -1; iVY <= YVertexCount; iVY++)
	{
		for(int32 iVX = -1; iVX <= XVertexCount; iVX++)
		{
			if(-1 < iVY && iVY < YVertexCount && -1 < iVX && iVX < XVertexCount)    // <--
			{
				SubVertices.Add(Vertices[VertexIndex]);
				SubUVs.Add(UVs[VertexIndex]);
				SubNormals.Add(Normals[VertexIndex]);
				SubTangents.Add(Tangents[VertexIndex]);
			}
			VertexIndex++;
		}
	}
	//Subset Triangles
	for(int32 iTY = 0; iTY <= YVertexCount -2; iTY++)
	{
		for(int32 iTX = 0; iTX <=XVertexCount - 2; iTX++)
		{
			SubTriangles.Add(iTX + iTY * XVertexCount);
			SubTriangles.Add(iTX + iTY * XVertexCount + XVertexCount);
			SubTriangles.Add(iTX + iTY * XVertexCount + 1);
			
			SubTriangles.Add(iTX + iTY * XVertexCount + XVertexCount);
			SubTriangles.Add(iTX + iTY * XVertexCount + XVertexCount +1);
			SubTriangles.Add(iTX + iTY * XVertexCount + 1);
		}
	}

	//Create Mesh

	TerrainMesh->CreateMeshSection(MeshSectionIndex, SubVertices, SubTriangles, SubNormals, SubUVs, TArray<FColor>(), SubTangents, true);
	if(TerrainMaterial)
	{
		TerrainMesh->SetMaterial(MeshSectionIndex, TerrainMaterial);
	}
	MeshSectionIndex++;
}

float AWorldGeneration::GetHeight(FVector2D Location)
{
	return 0.0f;
}

