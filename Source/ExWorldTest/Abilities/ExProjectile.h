// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "ExWorldTestPlayerController.h"
#include "ExProjectile.generated.h"


USTRUCT(BlueprintType, meta = (DisplayName = "Configure Data Affect"))
struct EXWORLDTEST_API FAffectDataTable : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FString AffectType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	int32 HpChange;

};

USTRUCT(BlueprintType)
struct FEffectData
{
	GENERATED_BODY()

		UPROPERTY()
		FString Type;

};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FApplyEffectToActor, class AActor*, TargetActor, const FEffectData&, Data);



UCLASS()
class EXWORLDTEST_API AExProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExProjectile();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true))
	float Range;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
	class USphereComponent* Collision;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Sphere;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneRoot;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UFUNCTION(BlueprintImplementableEvent)
	void OnApplyEffect(AActor* AffectedActor, const FEffectData& EffectData);


	UPROPERTY(BlueprintAssignable)
	FApplyEffectToActor ApplyEffectToActor;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIfDestroyAfterAffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumsOfAffectActor;

	UPROPERTY(ReplicatedUsing = OnRep_AlreadyAffectedNumsOfActors)
	int32 AlreadyAffectedNumsOfActors;
	UFUNCTION()
	void OnRep_AlreadyAffectedNumsOfActors();

public:

	/** called when something enters the sphere component */
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** called when something leaves the sphere component */
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
