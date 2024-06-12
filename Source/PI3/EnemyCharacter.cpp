// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "EnemyAnimInstance.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/BoxComponent.h"
#include "PI3Character.h"
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
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	TargetPlayer = Cast<API3Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
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
			AttackPlayer();
			LastAttackTime = GetWorld()->GetTimeSeconds();
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
		// Calcular la dirección hacia el jugador
		FVector Direction = TargetPlayer->GetActorLocation() - GetActorLocation();
		Direction.Z = 0; // Ignorar la diferencia en el eje Z para moverse en un plano 2D
		Direction.Normalize();

		// Actualizar MovementVector basado en la dirección calculada
		MovementVector = FVector2D(Direction.X, Direction.Y);
		
		// Aplicar movimiento directamente basado en la dirección hacia el jugador
		AddMovementInput(Direction);
	}
}

void AEnemyCharacter::AttackPlayer()
{
	if (TargetPlayer)
	{
		bIsAttacking = true;
		UE_LOG(LogTemp, Warning, TEXT("Enemy is attacking the player."))
		// Aplicar daño al jugador
		UGameplayStatics::ApplyDamage(TargetPlayer, 10.0f, GetController(), this, UDamageType::StaticClass());

		// Aquí puedes agregar lógica para efectos de sonido, partículas, etc.
	}
}


