// Copyright 2020 Dan Kestranek.


#include "ExProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AExProjectile::AExProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"), true);
	Collision->SetCollisionProfileName(FName("Collision"));
	Collision->AttachTo(RootComponent);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AExProjectile::OnOverlapBegin);


	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	Sphere->SetCollisionProfileName(FName("Sphere"));
	Sphere->AttachTo(Collision);

}

// Called when the game starts or when spawned
void AExProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AExProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* Owner2 = GetOwner();
	if (this == OtherActor)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("AExProjectile::OnOverlapBegin"));
}

void AExProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetOwner() == OtherActor)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("AExProjectile::OnOverlapEnd"));
}
