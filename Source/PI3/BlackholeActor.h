#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackholeActor.generated.h"

UCLASS()
class PI3_API ABlackholeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlackholeActor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Root = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BHMesh = nullptr;

protected:
	virtual void BeginPlay() override;
};
