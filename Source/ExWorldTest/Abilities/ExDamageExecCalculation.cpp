// Copyright 2020 Dan Kestranek.


#include "ExDamageExecCalculation.h"
#include "AttributeSets/ExAttributeSetBase.h"
#include "ExAbilitySystemComponent.h"


UExDamageExecCalculation::UExDamageExecCalculation(){}


void UExDamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters & ExecutionParams, OUT FGameplayEffectCustomExecutionOutput & OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	// need to change to random hp damage
	// Broadcast damages to Target ASC
	UExAbilitySystemComponent* TargetASC = Cast<UExAbilitySystemComponent>(TargetAbilitySystemComponent);
	if (TargetASC)
	{
		UExAbilitySystemComponent* SourceASC = Cast<UExAbilitySystemComponent>(SourceAbilitySystemComponent);
		TargetASC->ReceiveDamage(SourceASC, 1, 1);
	}
}
