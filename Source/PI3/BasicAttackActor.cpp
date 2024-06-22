#include "BasicAttackActor.h"
#include "BaseAttack.h"
#include "SpeedEnemy.h"
#include "TankEnemy.h"
#include "Engine/StaticMesh.h"

ABasicAttackActor::ABasicAttackActor()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    SphereCollider->SetupAttachment(RootComponent);
    SphereCollider->SetSphereRadius(250.f);
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
    if (OtherActor && (OtherActor != this))
    {
        ApplyDamage(OtherActor);
    }
}

void ABasicAttackActor::SetBasicAbility(UBaseAttack* Ability)
{
    BasicAttackAbility = Ability;
}

void ABasicAttackActor::ApplyDamage(AActor* EnemyActor)
{
    if (BasicAttackAbility)
    {
        float DamageAmount = BasicAttackAbility->DamageAmount;

        if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(EnemyActor))
        {
            EnemyCharacter->TakeDamage(DamageAmount);
        }
        if (ASpeedEnemy* SpeedEnemy = Cast<ASpeedEnemy>(EnemyActor))
        {
            SpeedEnemy->TakeDamage(DamageAmount);
        }
        if (ATankEnemy* TankEnemy = Cast<ATankEnemy>(EnemyActor))
        {
            TankEnemy->TakeDamage(DamageAmount);
        }
    }
}
