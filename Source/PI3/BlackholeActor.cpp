#include "BlackholeActor.h"
#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

ABlackholeActor::ABlackholeActor()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    SphereCollider->SetupAttachment(RootComponent);
    SphereCollider->SetSphereRadius(150.0f);
    SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereCollider->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
    SphereCollider->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    SphereCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ABlackholeActor::OnOverlapBegin);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
    if (SphereMeshAsset.Succeeded())
    {
        MeshComponent->SetStaticMesh(SphereMeshAsset.Object);
    }

    SphereSpeed = 1000.0f;
    MaxRange = 1500.0f;
}

void ABlackholeActor::BeginPlay()
{
    Super::BeginPlay();
}

void ABlackholeActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!IsValid(MeshComponent))
    {
        Destroy();
        return;
    }

    FVector NewLocation = GetActorLocation() + MovementDirection * SphereSpeed * DeltaTime;
    SetActorLocation(NewLocation);

    DistanceTraveled += SphereSpeed * DeltaTime;
    if (DistanceTraveled >= MaxRange)
    {
        Destroy();
    }
}

void ABlackholeActor::Initialize(const FVector& StartLocation, const FVector& Direction, float Range)
{
    SetActorLocation(StartLocation);
    MovementDirection = Direction.GetSafeNormal();
    MaxRange = Range;
    DistanceTraveled = 0.f;
}

void ABlackholeActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor))
    {
        ReduceEnemySpeed(EnemyCharacter);
        UE_LOG(LogTemp, Warning, TEXT("Overlapped with Enemy Character"));
    }
}

void ABlackholeActor::ReduceEnemySpeed(AEnemyCharacter* EnemyCharacter)
{
    if (EnemyCharacter)
    {
        float OriginalSpeed = EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed;
        float ReducedSpeed = OriginalSpeed * 0.5f;

        EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = ReducedSpeed;

        FTimerHandle SpeedRestoreTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(SpeedRestoreTimerHandle, [EnemyCharacter, OriginalSpeed]() {
            EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
        }, 15.0f, false);

        UE_LOG(LogTemp, Warning, TEXT("Reduced enemy speed"));
    }
}
