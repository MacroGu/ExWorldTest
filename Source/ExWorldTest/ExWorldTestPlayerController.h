// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ExWorldTestCharacter.h"
#include "ExWorldTestPlayerController.generated.h"




/**
 * 
 */
UCLASS()
class EXWORLDTEST_API AExWorldTestPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "GASDocumentation|UI")
	TSubclassOf<class UExShowTextWidgetComponent> DamageNumberClass;

	UFUNCTION(Client, Reliable, WithValidation)
	void ShowDamageNumber(float DamageAmount, AExWorldTestCharacter* TargetCharacter);
	void ShowDamageNumber_Implementation(float DamageAmount, AExWorldTestCharacter* TargetCharacter);
	bool ShowDamageNumber_Validate(float DamageAmount, AExWorldTestCharacter* TargetCharacter);

	// Simple way to RPC to the client the countdown until they respawn from the GameMode. Will be latency amount of out sync with the Server.
	UFUNCTION(Client, Reliable, WithValidation)
	void SetRespawnCountdown(float RespawnTimeRemaining);
	void SetRespawnCountdown_Implementation(float RespawnTimeRemaining);
	bool SetRespawnCountdown_Validate(float RespawnTimeRemaining);

protected:
	
	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;
};
