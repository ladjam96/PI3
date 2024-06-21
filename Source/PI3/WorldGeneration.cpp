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
            FMath::FloorToInt(Player->GetActorLocation().X / ((XVertexCount - 1) * CellSize)),
            FMath::FloorToInt(Player->GetActorLocation().Y / ((YVertexCount - 1) * CellSize))
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
            if (Player.IsValid())
            {
                UE_LOG(LogTemp, Warning, TEXT("Player found: %s"), *Player->GetName());
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Player not found!"));
            }
        }
    }
}

void AWorldGeneration::GenerateTerrain(const int SectionIndexX, const int SectionIndexY)
{
    FVector Offset = FVector(SectionIndexX * (XVertexCount - 1) * CellSize, SectionIndexY * (YVertexCount - 1) * CellSize, 0.f);

    UE_LOG(LogTemp, Warning, TEXT("Generating Terrain at Section (%d, %d) with Offset (%f, %f, %f)"), SectionIndexX, SectionIndexY, Offset.X, Offset.Y, Offset.Z);

    TArray<FVector> Vertices;
    TArray<FVector2D> UVs;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FProcMeshTangent> Tangents;

    // Vertices and UVs
    for (int32 iVY = 0; iVY < YVertexCount; ++iVY)
    {
        for (int32 iVX = 0; iVX < XVertexCount; ++iVX)
        {
            FVector Vertex;
            Vertex.X = iVX * CellSize + Offset.X;
            Vertex.Y = iVY * CellSize + Offset.Y;
            Vertex.Z = GetHeight(FVector2D(Vertex.X, Vertex.Y));
            Vertices.Add(Vertex);

            FVector2D UV;
            UV.X = static_cast<float>(iVX + SectionIndexX * (XVertexCount - 1)) / (NumOfSectionX * (XVertexCount - 1));
            UV.Y = static_cast<float>(iVY + SectionIndexY * (YVertexCount - 1)) / (NumOfSectionY * (YVertexCount - 1));
            UVs.Add(UV);
        }
    }

    // Triangles
    for (int32 iTY = 0; iTY < YVertexCount - 1; ++iTY)
    {
        for (int32 iTX = 0; iTX < XVertexCount - 1; ++iTX)
        {
            int32 V0 = iTX + iTY * XVertexCount;
            int32 V1 = V0 + 1;
            int32 V2 = V0 + XVertexCount;
            int32 V3 = V2 + 1;

            Triangles.Add(V0);
            Triangles.Add(V2);
            Triangles.Add(V1);

            Triangles.Add(V1);
            Triangles.Add(V2);
            Triangles.Add(V3);
        }
    }

    // Calculate Normals and Tangents
    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);

    // Create Mesh
    TerrainMesh->CreateMeshSection(MeshSectionIndex, Vertices, Triangles, Normals, UVs, TArray<FColor>(), Tangents, true);
    if (TerrainMaterial)
    {
        TerrainMesh->SetMaterial(MeshSectionIndex, TerrainMaterial);
    }

    UE_LOG(LogTemp, Warning, TEXT("Created Mesh Section %d"), MeshSectionIndex);

    MeshSectionIndex++;
}

float AWorldGeneration::GetHeight(FVector2D Location)
{
    return 0.0f; // Flat terrain for now
}

void AWorldGeneration::CheckAndGenerateNewTerrain()
{
    if (!Player.IsValid())
    {
        return;
    }

    FVector2D CurrentPlayerSection = FVector2D(
        FMath::FloorToInt(Player->GetActorLocation().X / ((XVertexCount - 1) * CellSize)),
        FMath::FloorToInt(Player->GetActorLocation().Y / ((YVertexCount - 1) * CellSize))
    );

    UE_LOG(LogTemp, Warning, TEXT("Player Section: (%f, %f)"), CurrentPlayerSection.X, CurrentPlayerSection.Y);

    if (CurrentPlayerSection != LastPlayerSection)
    {
        UE_LOG(LogTemp, Warning, TEXT("Generating new terrain sections around (%f, %f)"), CurrentPlayerSection.X, CurrentPlayerSection.Y);

        // Generate the central section and the surrounding sections
        GenerateTerrain(CurrentPlayerSection.X, CurrentPlayerSection.Y);
        GenerateTerrain(CurrentPlayerSection.X + 1, CurrentPlayerSection.Y);
        GenerateTerrain(CurrentPlayerSection.X - 1, CurrentPlayerSection.Y);
        GenerateTerrain(CurrentPlayerSection.X, CurrentPlayerSection.Y + 1);
        GenerateTerrain(CurrentPlayerSection.X, CurrentPlayerSection.Y - 1);
        GenerateTerrain(CurrentPlayerSection.X + 1, CurrentPlayerSection.Y + 1);
        GenerateTerrain(CurrentPlayerSection.X - 1, CurrentPlayerSection.Y - 1);
        GenerateTerrain(CurrentPlayerSection.X + 1, CurrentPlayerSection.Y - 1);
        GenerateTerrain(CurrentPlayerSection.X - 1, CurrentPlayerSection.Y + 1);

        LastPlayerSection = CurrentPlayerSection;
    }
}
