// Copyright 2020 Dan Kestranek.


#include "ExProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "ExWorldTestCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"




// Sets default values
AExProjectile::AExProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	
	AExWorldTestPlayerController* PC = Cast<AExWorldTestPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PC))
	{
		return;
	}

	ApplyEffectToActor.AddDynamic(this, &AExProjectile::OnApplyEffect);

}

void AExProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	AActor* tempActor = GetOwner();
	if (tempActor == OtherActor || OtherActor == this)
	{
		return;
	}

	if (AlreadyAffectedNumsOfActors >= NumsOfAffectActor)
	{
		return;
	}

	FEffectData Data;
	ApplyEffectToActor.Broadcast(OtherActor, Data);
	AlreadyAffectedNumsOfActors++;
	UE_LOG(LogTemp, Warning, TEXT("AExProjectile::OnOverlapEnd, %d"), AlreadyAffectedNumsOfActors);

}

void AExProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetOwner() == OtherActor)
	{
		return;
	}

	// UE_LOG(LogTemp, Warning, TEXT("AExProjectile::OnOverlapEnd"));

}


void AExProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExProjectile, AlreadyAffectedNumsOfActors);
}


void AExProjectile::OnRep_AlreadyAffectedNumsOfActors()
{

}
