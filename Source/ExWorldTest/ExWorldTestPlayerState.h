// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "ExWorldTestPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class EXWORLDTEST_API AExWorldTestPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AExWorldTestPlayerState();

	// Implement IAbilitySystemInterface
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UExAttributeSetBase* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "ExWorldTestPlayerState")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "ExWorldTestPlayerState|UI")
	void ShowAbilityConfirmCancelText(bool ShowText);


	/**
	* Getters for attributes from GDAttributeSetBase. Returns Current Value unless otherwise specified.
	*/

	UFUNCTION(BlueprintCallable, Category = "ExWorldTestPlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "ExWorldTestPlayerState|Attributes")
	float GetMaxHealth() const;

protected:
	UPROPERTY()
	class UExAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UExAttributeSetBase* AttributeSetBase;

	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);


};
