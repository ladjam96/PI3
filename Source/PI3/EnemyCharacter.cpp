// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "Animation/AnimInstance.h"
#include "PI3Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Move//
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	//Attack//
	bIsAttacking = false;
	AttackRange = 90.0f; // Ajusta el rango de ataque según sea necesario
	AttackCooldown = 3.0f; // Ajusta el tiempo de enfriamiento según sea necesario
	LastAttackTime = 0.0f;

	//Widget//
	MaxHealth = 100.0f; // Ejemplo: establece la salud máxima inicial
	CurrentHealth = MaxHealth; // Ejemplo: establece la salud actual inicial
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	TargetPlayer = Cast<API3Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent); // Adjuntar al componente raíz del personaje
	HealthBarWidgetComponent->SetWidgetClass(HealthBarWidgetClass);
	HealthBarWidgetComponent->SetDrawSize(FVector2D(200.f, 25.f)); // Tamaño del widget (ancho x alto)
	HealthBarWidgetComponent->SetVisibility(false); // Inicialmente oculto
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
			AttackPlayer();
			LastAttackTime = GetWorld()->GetTimeSeconds();
		}
		else
		{
			bIsAttacking = false;
		}
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

void AEnemyCharacter::Move()
{
	if (TargetPlayer && Controller != nullptr)
	{
		
		FVector Direction = TargetPlayer->GetActorLocation() - GetActorLocation();
		Direction.Z = 0; 
		Direction.Normalize();

		
		MovementVector = FVector2D(Direction.X, Direction.Y);
		
		
		AddMovementInput(Direction);
	}
}

void AEnemyCharacter::AttackPlayer()
{
	if (TargetPlayer && AttackMontage && !bIsAttacking)
	{
		bIsAttacking = true;

		if (bIsAttacking == true)
		{
			PlayAnimMontage(AttackMontage);

			// Aplicar daño al jugador
			//UGameplayStatics::ApplyDamage(TargetPlayer, 10.0f, GetController(), this, UDamageType::StaticClass());
		}
	}
}

/*void AEnemyCharacter::UpdateHealthBar(float NewHealth, float MaxHealth)
{
	if (HealthBarWidgetComponent && HealthBarWidgetComponent->GetUserWidgetObject())
	{
		UUserWidget* HealthBarWidget = HealthBarWidgetComponent->GetUserWidgetObject();
		// Asumiendo que tienes una función UpdateHealth en el widget para actualizar la barra de vida
		HealthBarWidget->CallFunctionByNameWithArguments(TEXT("UpdateHealth"), ar, nullptr, true);
	}
}*/
