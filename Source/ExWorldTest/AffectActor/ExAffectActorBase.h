
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "ExWorldTestPlayerController.h"
#include "ExAffectActorBase.generated.h"



UCLASS()
class EXWORLDTEST_API AExAffectActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExAffectActorBase();




public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString AffectType;




};
