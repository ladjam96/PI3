#include "BasicAttackActor.h"
#include "BaseAttack.h" // Include BaseAttack header for UBaseAttack class
#include "Engine/StaticMesh.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABasicAttackActor::ABasicAttackActor()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    SphereCollider->SetupAttachment(RootComponent);
    SphereCollider->SetSphereRadius(150.f);
    SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereCollider->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
    SphereCollider->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    SphereCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ABasicAttackActor::OnOverlapBegin);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
    if (SphereMeshAsset.Succeeded())
    {
        MeshComponent->SetStaticMesh(SphereMeshAsset.Object);
    }

    SphereSpeed = 1500.f;
    MaxRange = 2000.f;
}

void ABasicAttackActor::BeginPlay()
{
    Super::BeginPlay();
}

void ABasicAttackActor::Tick(float DeltaTime)
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

void ABasicAttackActor::Initialize(const FVector& StartLocation, const FVector& Direction, float Range)
{
    SetActorLocation(StartLocation);
    MovementDirection = Direction.GetSafeNormal();
    MaxRange = Range;
    DistanceTraveled = 0.f;
}

void ABasicAttackActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor))
    {
        ApplyDamage(EnemyCharacter);
    }
}

void ABasicAttackActor::SetBasicAbility(UBaseAttack* Ability)
{
    BasicAttackAbility = Ability;
}

void ABasicAttackActor::ApplyDamage(AEnemyCharacter* EnemyCharacter)
{
    if (EnemyCharacter && BasicAttackAbility)
    {
        float DamageAmount = BasicAttackAbility->DamageAmount;
        EnemyCharacter->TakeDamage(DamageAmount);
    }
}
