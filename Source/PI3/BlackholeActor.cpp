#include "BlackholeActor.h"

ABlackholeActor::ABlackholeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	BHMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshCmp"));
	BHMesh->SetupAttachment(Root);
}

void ABlackholeActor::BeginPlay()
{
	Super::BeginPlay();

	
}

void ABlackholeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

