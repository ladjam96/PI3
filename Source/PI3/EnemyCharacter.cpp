// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
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

	// Configurar movimiento del personaje
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	//DetectPlayerCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));

	//DetectPlayerCollisionSphere->SetupAttachment(RootComponent);
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

/*USphereComponent* AEnemyCharacter::DetectPlayerCollisionSphere()
{
	//return DetectPlayerCollisionSphere;
}*/


