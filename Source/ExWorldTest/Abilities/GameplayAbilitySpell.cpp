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




UGameplayAbilitySpell::UGameplayAbilitySpell()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTag SpellAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Spell"));
	AbilityTags.AddTag(SpellAbilityTag);
	ActivationOwnedTags.AddTag(SpellAbilityTag);

	Range = 1000.0f;
	Damage = 12.0f;
}

bool UGameplayAbilitySpell::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags , OUT FGameplayTagContainer* OptionalRelevantTags ) const
{

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	AExWorldTestCharacter* Hero = Cast<AExWorldTestCharacter>(GetAvatarActorFromActorInfo());
	if (!Hero)
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

	UExAbilityTask_PlayMontageAndWaitForEvent* Task = UExAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, SpellMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);

	Task->OnBlendOut.AddDynamic(this, &UGameplayAbilitySpell::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UGameplayAbilitySpell::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGameplayAbilitySpell::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UGameplayAbilitySpell::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UGameplayAbilitySpell::EventReceived);
	Task->ReadyForActivation();

}

void UGameplayAbilitySpell::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGameplayAbilitySpell::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbilitySpell::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (GetOwningActorFromActorInfo()->GetLocalRole() != ROLE_Authority || EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Montage.SpawnProjectile")))
	{
		return;
	}

	AExWorldTestCharacter* Hero = Cast<AExWorldTestCharacter>(GetAvatarActorFromActorInfo());
	if (!Hero)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	FActorSpawnParameters params;
	params.Owner = Hero;
	params.Instigator = Hero;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform BulletSpawnTransform;
	BulletSpawnTransform.SetLocation(Hero->GetActorLocation());
	BulletSpawnTransform.SetRotation(Hero->GetActorRotation().Quaternion());

	AExProjectile* Projectile = GetWorld()->SpawnActor<AExProjectile>(ProjectileClass, BulletSpawnTransform, params);
	Projectile->Range = Range;
}
