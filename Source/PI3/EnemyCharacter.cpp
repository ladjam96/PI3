#include "EnemyCharacter.h"
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

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = 400.0f;

    bIsAttacking = false;
    bIsDead = false;
    AttackRange = 100.0f;
    AttackCooldown = 3.0f;
    LastAttackTime = 0.0f;
    Damage = 20.f;

    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;

    HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
    HealthBarWidgetComponent->SetupAttachment(RootComponent);
    HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthBarWidgetComponent->SetDrawSize(FVector2D(100, 20));

    XP = 10.f;
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    TargetPlayer = Cast<API3Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    
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

        const float DistanceToPlayer = FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());

        if (DistanceToPlayer <= AttackRange && GetWorld()->GetTimeSeconds() - LastAttackTime >= AttackCooldown && bIsDead == false)
        {
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
    if (CurrentHealth <= 0 && bIsDead == false)
    {
        bIsDead = true;
        Die();
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

    if (HealthBarWidget)
    {
        HealthBarWidget->UpdateHealthBar(CurrentHealth, MaxHealth);
    }
}

void AEnemyCharacter::Die()
{
    if (DeathMontage && bIsDead == true)
    {
        PlayAnimMontage(DeathMontage);
        
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetCharacterMovement()->DisableMovement();

        if (API3Character* PlayerCharacter = Cast<API3Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
        {
            PlayerCharacter->GainExperience(XP);
        }
        
        SetLifeSpan(1.f);
        bIsAttacking = false;
    }
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

void AEnemyCharacter::ScaleStatsWithPlayerLevel(int32 PlayerLevel)
{
    float ScalingFactor = 1.0f + (PlayerLevel * 0.05f);

    Damage *= ScalingFactor;
    MaxHealth *= ScalingFactor;

    CurrentHealth = MaxHealth;
}