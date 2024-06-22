

#include "TankEnemy.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ATankEnemy::ATankEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	bIsAttacking = false;
	bIsDead = false;
	AttackRange = 300.0f;
	AttackCooldown = 5.0f;
	LastAttackTime = 0.0f;
	Damage = 100.f;
	
	MaxHealth = 500.0f;
	CurrentHealth = MaxHealth;

	// Health Bar Widget
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetDrawSize(FVector2D(100, 20));

	XP = 70.f;
}

void ATankEnemy::BeginPlay()
{
	Super::BeginPlay();

	TargetPlayer = Cast<API3Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    
	if (HealthBarWidgetComponent)
	{
		HealthBarWidget = Cast<UEnemyHealthBar>(HealthBarWidgetComponent->GetUserWidgetObject());
	}
}

void ATankEnemy::Tick(float DeltaTime)
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

void ATankEnemy::Move()
{
	if (TargetPlayer && Controller != nullptr)
	{
		FVector Direction = TargetPlayer->GetActorLocation() - GetActorLocation();
		Direction.Z = 0;
		Direction.Normalize();

		AddMovementInput(Direction);
	}
}

void ATankEnemy::AttackPlayer(float DamageAmount)
{
	if (TargetPlayer && AttackMontage && !bIsAttacking)
	{
		bIsAttacking = true;
		PlayAnimMontage(AttackMontage);
		TargetPlayer->TakeDamage(DamageAmount);
	}
}

void ATankEnemy::TakeDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;

	if (HealthBarWidget)
	{
		HealthBarWidget->UpdateHealthBar(CurrentHealth, MaxHealth);
	}
}

void ATankEnemy::Die()
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
    
		SetLifeSpan(2.50f);
		bIsAttacking = false;
	}
}

FVector ATankEnemy::GetCharacterVelocity() const
{
	return GetVelocity();
}

FRotator ATankEnemy::GetCharacterDirection() const
{
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		return FRotator(0.0f, Rotation.Yaw, 0.0f);
	}
	return FRotator::ZeroRotator;
}

void ATankEnemy::ScaleStatsWithPlayerLevel(int32 PlayerLevel)
{
	float ScalingFactor = 1.0f + (PlayerLevel * 0.05f);

	Damage *= ScalingFactor;
	MaxHealth *= ScalingFactor;

	CurrentHealth = MaxHealth;
}
