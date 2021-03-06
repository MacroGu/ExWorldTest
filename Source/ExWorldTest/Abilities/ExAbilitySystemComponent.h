// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ExWorldTest.h"
#include "AbilitySystemComponent.h"
#include "ExGameplayAbility.h"
#include "ExAbilitySystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UExAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);


/**
 * Subclass of ability system component with game-specific data
 * Most games will need to make a game-specific subclass to provide utility functions
 */

UCLASS()
class EXWORLDTEST_API UExAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamage;

	// Called from ExDamageExecCalculation. Broadcasts on ReceivedDamage whenever this ASC receives damage.
	virtual void ReceiveDamage(UExAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);


};