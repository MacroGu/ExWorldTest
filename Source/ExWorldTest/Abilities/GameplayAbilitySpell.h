#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "ExGameplayAbility.h"
#include "GameplayAbilitySpell.generated.h"


class AExProjectile;

/**
 *	Ability that Spell.
 */

UCLASS()
class EXWORLDTEST_API UGameplayAbilitySpell : public UExGameplayAbility
{
	GENERATED_BODY()

public:

	UGameplayAbilitySpell();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* SpellMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AExProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Range;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Damage;

	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

};
