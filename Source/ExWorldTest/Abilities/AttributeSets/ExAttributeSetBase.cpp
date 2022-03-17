// Copyright 2020 Dan Kestranek.


#include "ExAttributeSetBase.h"
#include "ExWorldTestCharacter.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UExAttributeSetBase::UExAttributeSetBase()
{
	// Cache tags

}

void UExAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// This is called whenever attributes change, so for max health/mana we want to scale the current totals to match
	Super::PreAttributeChange(Attribute, NewValue);


	UE_LOG(LogTemp, Warning, TEXT("UExAttributeSetBase::PreAttributeChange"));

}

void UExAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);


	UE_LOG(LogTemp, Warning, TEXT("UExAttributeSetBase::PostGameplayEffectExecute"));
}

void UExAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UExAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UExAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UExAttributeSetBase, LastSpellTime, COND_None, REPNOTIFY_Always);
}


void UExAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExAttributeSetBase, Health, OldHealth);
}

void UExAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExAttributeSetBase, MaxHealth, OldMaxHealth);
}

void UExAttributeSetBase::OnRep_LastSpellTime(const FGameplayAttributeData& OldLastSpellTime)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExAttributeSetBase, LastSpellTime, OldLastSpellTime);
}
