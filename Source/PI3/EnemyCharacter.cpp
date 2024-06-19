#include "EnemyCharacter.h"
#include "Animation/AnimInstance.h"
#include "PI3Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyCharacter::AEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = 400.0f;

    // Combat
    bIsAttacking = false;
    AttackRange = 90.0f;
    AttackCooldown = 3.0f;
    LastAttackTime = 0.0f;
    Damage = 20.f;

    // Health
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;

    // Health Bar Widget
    HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
    HealthBarWidgetComponent->SetupAttachment(RootComponent);
    HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthBarWidgetComponent->SetDrawSize(FVector2D(100, 20));

   
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    TargetPlayer = Cast<API3Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    // Retrieve the widget object after BeginPlay to ensure it's initialized correctly
    if (HealthBarWidgetComponent)
    {
        HealthBarWidget = Cast<UEnemyHealthBar>(HealthBarWidgetComponent->GetUserWidgetObject());
    }
}

void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TargetPlayer)
    {
        Move();

        float DistanceToPlayer = FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());

        if (DistanceToPlayer <= AttackRange && GetWorld()->GetTimeSeconds() - LastAttackTime >= AttackCooldown)
        {
            UE_LOG(LogTemp, Warning, TEXT("Enemy is within attack range."));
            AttackPlayer(Damage);
            LastAttackTime = GetWorld()->GetTimeSeconds();
        }
        else
        {
            bIsAttacking = false;
        }
    }

    if (HealthBarWidget)
    {
        HealthBarWidget->UpdateHealthBar(CurrentHealth, MaxHealth);
    }
}

void AEnemyCharacter::Move()
{
    if (TargetPlayer && Controller != nullptr)
    {
        FVector Direction = TargetPlayer->GetActorLocation() - GetActorLocation();
        Direction.Z = 0;
        Direction.Normalize();

        AddMovementInput(Direction);
    }
}

void AEnemyCharacter::AttackPlayer(float DamageAmount)
{
    if (TargetPlayer && AttackMontage && !bIsAttacking)
    {
        bIsAttacking = true;
        PlayAnimMontage(AttackMontage);
        TargetPlayer->TakeDamage(DamageAmount);
    }
}

void AEnemyCharacter::TakeDamage(float DamageAmount)
{
    CurrentHealth -= DamageAmount;

    if (CurrentHealth <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("1"));
        Die();
    }

    if (HealthBarWidget)
    {
        HealthBarWidget->UpdateHealthBar(CurrentHealth, MaxHealth);
    }
}

void AEnemyCharacter::Die()
{
    if (CurrentHealth <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("2"));
        PlayAnimMontage(DeathMontage);
        // Additional logic for effects like sound, particles, etc. can be added here.
    }

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(5.0f);
}

FVector AEnemyCharacter::GetCharacterVelocity() const
{
    return GetVelocity();
}

FRotator AEnemyCharacter::GetCharacterDirection() const
{
    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        return FRotator(0.0f, Rotation.Yaw, 0.0f);
    }
    return FRotator::ZeroRotator;
}
