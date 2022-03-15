#include "GameplayAbilitySpell.h"
#include "Tasks/ExAbilityTask_PlayMontageAndWaitForEvent.h"
#include "ExWorldTestCharacter.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ExProjectile.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"




UGameplayAbilitySpell::UGameplayAbilitySpell(const FObjectInitializer& ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

bool UGameplayAbilitySpell::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UGameplayAbilitySpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}


	UExAbilityTask_PlayMontageAndWaitForEvent* Task = UExAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, FName(TEXT("None")), SpellMontage, AbilityTags);


	Task->OnBlendOut.AddDynamic(this, &UGameplayAbilitySpell::MontagePlayEnd);
	Task->OnInterrupted.AddDynamic(this, &UGameplayAbilitySpell::MontagePlayEnd);
	Task->OnCancelled.AddDynamic(this, &UGameplayAbilitySpell::MontagePlayEnd);
	Task->EventReceived.AddDynamic(this, &UGameplayAbilitySpell::MontagePlayEventReceived);
	Task->ReadyForActivation();

}


void UGameplayAbilitySpell::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ApplyCooldown(Handle, ActorInfo, ActivationInfo);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbilitySpell::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ApplyCost(Handle, ActorInfo, ActivationInfo);
}


void UGameplayAbilitySpell::MontagePlayEnd(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbilitySpell::MontagePlayEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

	Super::ApplyEffectContainer(EventTag, EventData, -1);

}
