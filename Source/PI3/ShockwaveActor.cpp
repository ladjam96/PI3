#include "ShockwaveActor.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"

AShockwaveActor::AShockwaveActor()
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = Root;

    SWmesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SWMesh"));
    SWmesh->SetupAttachment(RootComponent);

    SWCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SWCollider"));
    SWCollider->SetupAttachment(RootComponent);
    SWCollider->SetSphereRadius(300.0f); // Adjust the radius as needed
    SWCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // QueryOnly for overlap detection
    SWCollider->SetCollisionResponseToAllChannels(ECR_Overlap);

    NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    NiagaraComp->SetupAttachment(RootComponent); // Attach Niagara component to the root
    NiagaraComp->SetAutoActivate(false); // Initially deactivate Niagara system

    SWCollider->OnComponentBeginOverlap.AddDynamic(this, &AShockwaveActor::OnOverlapBegin);
}

void AShockwaveActor::BeginPlay()
{
    Super::BeginPlay();
}

void AShockwaveActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AShockwaveActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                                     bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        // Log the name of the overlapping actor and component
        UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor: %s"), *OtherActor->GetName());
        UE_LOG(LogTemp, Warning, TEXT("Overlapping Component: %s"), *OtherComp->GetName());

        // Example: Activate the Niagara system when overlap occurs
        ActivateNiagaraSystem();
    }
}

void AShockwaveActor::ActivateNiagaraSystem()
{
    if (NiagaraComp)
    {
        // Activate the Niagara system
        NiagaraComp->Activate();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Niagara Component not found on AShockwaveActor"));
    }
}
