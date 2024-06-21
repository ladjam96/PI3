#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "BasicAttackActor.generated.h"

class UBaseAttack;

UCLASS()
class PI3_API ABasicAttackActor : public AActor
{
	GENERATED_BODY()
    
public: 
	ABasicAttackActor();

	virtual void Tick(float DeltaTime) override;

	void Initialize(const FVector& StartLocation, const FVector& Direction, float Range);

	void SetBasicAbility(UBaseAttack* Ability);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void ApplyDamage(AActor* EnemyActor);
    
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereCollider = nullptr;

	FVector MovementDirection {0};
    
	UPROPERTY(EditAnywhere)
	float SphereSpeed = 0.f;
    
	UPROPERTY(EditAnywhere)
	float DistanceTraveled = 0.f;
    
	UPROPERTY(EditAnywhere)
	float MaxRange = 0.f;

	UPROPERTY()
	UBaseAttack* BasicAttackAbility = nullptr; 
};
