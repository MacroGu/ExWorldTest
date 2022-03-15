// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExAbilitySystemComponent.h"
#include "ExGameplayAbility.h"
#include "AbilitySystemGlobals.h"


void UExAbilitySystemComponent::ReceiveDamage(UExAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
