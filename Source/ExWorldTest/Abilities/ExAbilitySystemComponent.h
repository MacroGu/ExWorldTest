// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ExWorldTest.h"
#include "AbilitySystemComponent.h"
#include "ExGameplayAbility.h"
#include "ExAbilitySystemComponent.generated.h"


/**
 * Subclass of ability system component with game-specific data
 * Most games will need to make a game-specific subclass to provide utility functions
 */

UCLASS()
class EXWORLDTEST_API UExAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Constructors and overrides
	UExAbilitySystemComponent();

	/** Returns a list of currently active ability instances that match the tags */
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UExGameplayAbility*>& ActiveAbilities);

	/** Returns the default level used for ability activations, derived from the character */
	int32 GetDefaultAbilityLevel() const;

	/** Version of function in AbilitySystemGlobals that returns correct type */
	static UExAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);


	void GrantInitialAbilities();


public:

	// Initial abilities for this Character
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<FAbilityBindInfo> InitialAbilities;


protected:
	bool bCharacterAbilitiesGiven = false;


};