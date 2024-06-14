#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShockwaveActor.generated.h"

class UNiagaraComponent;
class UTimelineComponent;
class USphereComponent;

UCLASS()
class PI3_API AShockwaveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AShockwaveActor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Root = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* SWmesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SWCollider = nullptr;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* NiagaraComp; // Niagara component for the shockwave effect

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void ActivateNiagaraSystem();

protected:
	virtual void BeginPlay() override;
};
