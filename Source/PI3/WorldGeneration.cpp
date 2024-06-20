#include "WorldGeneration.h"
#include "KismetProceduralMeshLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

AWorldGeneration::AWorldGeneration()
{
    PrimaryActorTick.bCanEverTick = true;

    TerrainMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainMesh"));
    TerrainMesh->SetupAttachment(GetRootComponent()); 
}

void AWorldGeneration::BeginPlay()
{
    Super::BeginPlay();
    
    FindPlayer();

    if (Player.IsValid())
    {
        LastPlayerSection = FVector2D(
            FMath::FloorToInt(Player->GetActorLocation().X / (XVertexCount * CellSize)),
            FMath::FloorToInt(Player->GetActorLocation().Y / (YVertexCount * CellSize))
        );

        GenerateTerrain(LastPlayerSection.X, LastPlayerSection.Y);
    }
}

void AWorldGeneration::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!Player.IsValid())
    {
        FindPlayer();
    }
    
    if (Player.IsValid())
    {
        CheckAndGenerateNewTerrain();
    }
}

void AWorldGeneration::FindPlayer()
{
    if (!Player.IsValid())
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            Player = PlayerController->GetPawn();
        }
    }
}

void AWorldGeneration::GenerateTerrain(const int SectionIndexX, const int SectionIndexY)
{
    FVector Offset = FVector(SectionIndexX * (XVertexCount - 1), SectionIndexY * (YVertexCount - 1), 0.f) * CellSize;

    TArray<FVector> Vertices;
    FVector Vertex;

    TArray<FVector2D> UVs;
    FVector2D UV;

    TArray<int32> Triangles;

    TArray<FVector> Normals;
    TArray<FProcMeshTangent> Tangents;

    // Vertices and UVs
    for (int32 iVY = 0; iVY <= YVertexCount; iVY++)
    {
        for (int32 iVX = 0; iVX <= XVertexCount; iVX++)
        {
            Vertex.X = iVX * CellSize + Offset.X;
            Vertex.Y = iVY * CellSize + Offset.Y;
            Vertex.Z = GetHeight(FVector2D(Vertex.X, Vertex.Y));
            Vertices.Add(Vertex);

            UV.X = iVX / (float)XVertexCount;
            UV.Y = iVY / (float)YVertexCount;
            UVs.Add(UV);
        }
    }

    // Triangles
    for (int32 iTY = 0; iTY < YVertexCount; iTY++)
    {
        for (int32 iTX = 0; iTX < XVertexCount; iTX++)
        {
            Triangles.Add(iTX + iTY * (XVertexCount + 1));
            Triangles.Add(iTX + (iTY + 1) * (XVertexCount + 1));
            Triangles.Add(iTX + iTY * (XVertexCount + 1) + 1);

            Triangles.Add(iTX + (iTY + 1) * (XVertexCount + 1));
            Triangles.Add(iTX + (iTY + 1) * (XVertexCount + 1) + 1);
            Triangles.Add(iTX + iTY * (XVertexCount + 1) + 1);
        }
    }

    // Calculate Normals
    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);

    // Create Mesh
    TerrainMesh->CreateMeshSection(MeshSectionIndex, Vertices, Triangles, Normals, UVs, TArray<FColor>(), Tangents, true);
    if (TerrainMaterial)
    {
        TerrainMesh->SetMaterial(MeshSectionIndex, TerrainMaterial);
    }
    MeshSectionIndex++;
}

float AWorldGeneration::GetHeight(FVector2D Location)
{
    return 0.0f; // Placeholder for height calculation
}

void AWorldGeneration::CheckAndGenerateNewTerrain()
{
    FVector2D CurrentPlayerSection = FVector2D(
        FMath::FloorToInt(Player->GetActorLocation().X / (XVertexCount * CellSize)),
        FMath::FloorToInt(Player->GetActorLocation().Y / (YVertexCount * CellSize))
    );

    if (CurrentPlayerSection != LastPlayerSection)
    {
        GenerateTerrain(CurrentPlayerSection.X, CurrentPlayerSection.Y);

        // Optionally generate surrounding sections
        GenerateTerrain(CurrentPlayerSection.X + 1, CurrentPlayerSection.Y);
        GenerateTerrain(CurrentPlayerSection.X - 1, CurrentPlayerSection.Y);
        GenerateTerrain(CurrentPlayerSection.X, CurrentPlayerSection.Y + 1);
        GenerateTerrain(CurrentPlayerSection.X, CurrentPlayerSection.Y - 1);

        LastPlayerSection = CurrentPlayerSection;
    }
}
