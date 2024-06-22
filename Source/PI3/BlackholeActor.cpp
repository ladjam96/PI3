#include "BlackholeActor.h"
#include "BlackHoleAbility.h"
#include "EnemyCharacter.h"
#include "SpeedEnemy.h"
#include "TankEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

ABlackholeActor::ABlackholeActor()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    SphereCollider->SetupAttachment(RootComponent);
    SphereCollider->SetSphereRadius(250.0f);
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
    if (OtherActor && (OtherActor != this))
    {
        if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor))
        {
            if(IsValid(EnemyCharacter))
                ReduceEnemySpeed(EnemyCharacter);
        }
        else if (ASpeedEnemy* SpeedEnemy = Cast<ASpeedEnemy>(OtherActor))
        {
            if(IsValid(SpeedEnemy))
                ReduceEnemySpeed(SpeedEnemy);
        }
        else if (ATankEnemy* TankEnemy = Cast<ATankEnemy>(OtherActor))
        {
            if(IsValid(TankEnemy))
                ReduceEnemySpeed(TankEnemy);
        }

        ApplyDamage(OtherActor);
    }
}

void ABlackholeActor::ReduceEnemySpeed(AActor* EnemyActor)
{
    FTimerHandle SpeedRestoreTimerHandle;

    if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(EnemyActor))
    {
        float OriginalSpeed = EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed;
        float ReducedSpeed = OriginalSpeed * 0.5f;

        EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = ReducedSpeed;

        GetWorld()->GetTimerManager().SetTimer(SpeedRestoreTimerHandle, [EnemyCharacter, OriginalSpeed]()
            {
                EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
            }, 5.0f, false);
    }
    else if (ASpeedEnemy* SpeedEnemy = Cast<ASpeedEnemy>(EnemyActor))
    {
        float OriginalSpeed = SpeedEnemy->GetCharacterMovement()->MaxWalkSpeed;
        float ReducedSpeed = OriginalSpeed * 0.5f;

        SpeedEnemy->GetCharacterMovement()->MaxWalkSpeed = ReducedSpeed;

        GetWorld()->GetTimerManager().SetTimer(SpeedRestoreTimerHandle, [SpeedEnemy, OriginalSpeed]()
            {
                SpeedEnemy->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
            }, 15.0f, false);
    }
    else if (ATankEnemy* TankEnemy = Cast<ATankEnemy>(EnemyActor))
    {
        float OriginalSpeed = TankEnemy->GetCharacterMovement()->MaxWalkSpeed;
        float ReducedSpeed = OriginalSpeed * 0.5f;

        TankEnemy->GetCharacterMovement()->MaxWalkSpeed = ReducedSpeed;

        GetWorld()->GetTimerManager().SetTimer(SpeedRestoreTimerHandle, [TankEnemy, OriginalSpeed]()
        {
            TankEnemy->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
        }, 15.0f, false);
    }
}

void ABlackholeActor::SetBlackHoleAbility(UBlackHoleAbility* Ability)
{
    BlackHoleAbility = Ability;
}

void ABlackholeActor::ApplyDamage(AActor* EnemyActor)
{
    if (BlackHoleAbility)
    {
        float DamageAmount = BlackHoleAbility->DamageAmount;

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