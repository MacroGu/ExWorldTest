// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExAbilitySystemComponent.h"
#include "ExGameplayAbility.h"
#include "AbilitySystemGlobals.h"



UExAbilitySystemComponent::UExAbilitySystemComponent() {}

void UExAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UExGameplayAbility*>& ActiveAbilities)
{
	//TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	//GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	//// Iterate the list of all ability specs
	//for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	//{
	//	// Iterate all instances on this ability spec
	//	TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

	//	for (UGameplayAbility* ActiveAbility : AbilityInstances)
	//	{
	//		ActiveAbilities.Add(Cast<URPGGameplayAbility>(ActiveAbility));
	//	}
	//}
}

int32 UExAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	//ARPGCharacterBase* OwningCharacter = Cast<ARPGCharacterBase>(GetOwnerActor());

	//if (OwningCharacter)
	//{
	//	return OwningCharacter->GetCharacterLevel();
	//}
	return 1;
}

UExAbilitySystemComponent* UExAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	// return Cast<UExAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
	return nullptr;
}

void UExAbilitySystemComponent::GrantInitialAbilities()
{
	if (bCharacterAbilitiesGiven)
	{
		return;
	}

	for (const FAbilityBindInfo& BindInfo : InitialAbilities)
	{
		if (BindInfo.GameplayAbilityClass)
		{
			GiveAbility(FGameplayAbilitySpec(BindInfo.GameplayAbilityClass, 1, static_cast<int32>(BindInfo.Command)));
		}
	}

	bCharacterAbilitiesGiven = true;
}