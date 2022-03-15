#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "ExGameplayAbility.h"
#include "GameplayAbilitySpell.generated.h"


/**
 *	Ability that Spell.
 */

UCLASS()
class EXWORLDTEST_API UGameplayAbilitySpell : public UExGameplayAbility
{
	GENERATED_BODY()

public:

	UGameplayAbilitySpell(const FObjectInitializer& ObjectInitializer);

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;



public:

	UFUNCTION()
	void MontagePlayEnd(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void MontagePlayEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);


public:

	UPROPERTY(EditDefaultsOnly, Category = Skills)
	UAnimMontage* SpellMontage;

};
