// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ExWorldTestCharacter.h"
#include "ExWorldTestPlayerController.generated.h"



class UExTimeLimitationShow;

/**
 * 
 */
UCLASS()
class EXWORLDTEST_API AExWorldTestPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AExWorldTestPlayerController();

	UPROPERTY(EditAnywhere, Category = "ExWorldTest|UI")
	TSubclassOf<UExTimeLimitationShow> TimeLimitationClass;


	void ShowTimeLimitation(const int64 TimeLeft);


protected:
	
	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;

	UPROPERTY(BlueprintReadWrite, Category = "GASDocumentation|UI")
	UExTimeLimitationShow* UITimeLimitation;

};
