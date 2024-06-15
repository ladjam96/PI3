#include "BlackholeActor.h"

ABlackholeActor::ABlackholeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(RootComponent);
	SphereCollider->SetSphereRadius(150.0f);
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollider->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	SphereCollider->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	SphereCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ABlackholeActor::OnOverlapBegin);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(SphereMeshAsset.Object);
	}

	SphereSpeed = 1000.0f;
	MaxRange = 1500.0f;
}

void ABlackholeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlackholeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(MeshComponent))
	{
		Destroy();
		return;
	}

	FVector NewLocation = GetActorLocation() + MovementDirection * SphereSpeed * DeltaTime;
	SetActorLocation(NewLocation);

	DistanceTraveled += SphereSpeed * DeltaTime;
	if (DistanceTraveled >= MaxRange)
	{
		Destroy();
	}
}

void ABlackholeActor::Initialize(const FVector& StartLocation, const FVector& Direction, float Range)
{
	SetActorLocation(StartLocation);
	MovementDirection = Direction.GetSafeNormal();
	MaxRange = Range;
	DistanceTraveled = 0.f;
}

void ABlackholeActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		// Implement your overlap behavior, such as applying forces or other effects
	}
}
