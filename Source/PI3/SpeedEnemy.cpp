#include "SpeedEnemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASpeedEnemy::ASpeedEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

	bIsAttacking = false;
	bIsDead = false;
	AttackRange = 120.0f;
	AttackCooldown = 1.0f;
	LastAttackTime = 0.0f;
	Damage = 50.f;

	MaxHealth = 50.0f;
	CurrentHealth = MaxHealth;

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetDrawSize(FVector2D(100, 20));

	XP = 30.f;
}

void ASpeedEnemy::BeginPlay()
{
	Super::BeginPlay();

	TargetPlayer = Cast<API3Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    
	if (HealthBarWidgetComponent)
	{
		HealthBarWidget = Cast<UEnemyHealthBar>(HealthBarWidgetComponent->GetUserWidgetObject());
	}
}

void ASpeedEnemy::Tick(float DeltaTime)
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

void ASpeedEnemy::Move()
{
	if (TargetPlayer && Controller != nullptr)
	{
		FVector Direction = TargetPlayer->GetActorLocation() - GetActorLocation();
		Direction.Z = 0;
		Direction.Normalize();

		AddMovementInput(Direction);
	}
}

void ASpeedEnemy::AttackPlayer(float DamageAmount)
{
	if (TargetPlayer && AttackMontage && !bIsAttacking)
	{
		bIsAttacking = true;
		PlayAnimMontage(AttackMontage);
		TargetPlayer->TakeDamage(DamageAmount);
	}
}

void ASpeedEnemy::TakeDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;

	if (HealthBarWidget)
	{
		HealthBarWidget->UpdateHealthBar(CurrentHealth, MaxHealth);
	}
}

void ASpeedEnemy::Die()
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
    
		SetLifeSpan(0.50f);
		bIsAttacking = false;
	}
}

FVector ASpeedEnemy::GetCharacterVelocity() const
{
	return GetVelocity();
}

FRotator ASpeedEnemy::GetCharacterDirection() const
{
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		return FRotator(0.0f, Rotation.Yaw, 0.0f);
	}
	return FRotator::ZeroRotator;
}

void ASpeedEnemy::ScaleStatsWithPlayerLevel(int32 PlayerLevel)
{
	float ScalingFactor = 1.0f + (PlayerLevel * 0.05f);

	Damage *= ScalingFactor;
	MaxHealth *= ScalingFactor;

	CurrentHealth = MaxHealth;
}


