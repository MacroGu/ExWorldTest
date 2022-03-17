// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/ExAbilitySystemComponent.h"
#include "ExWorldTestCharacter.generated.h"


class UExCharacterStatusBarWidget;

UCLASS(config=Game)
class AExWorldTestCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AExWorldTestCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// Implement IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UExCharacterStatusBarWidget* GetExPlayerStatusBar() { return UIExCharacterStatusBarWidget; }

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	bool ASCInputBound = false;

	FGameplayTag DeadTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AffectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CoolDownTimeForSpellSkill;


public:
	// rpcs 
	// change health 
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ChangeHealth(float NewValue);
	void ChangeHealth_Implementation(float NewValue);
	bool ChangeHealth_Validate(float NewValue);

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION()
	void InitializeCharacterStatusBar();

	// Client only
	virtual void OnRep_PlayerState() override;

	void BindASCInput();


public:
	UFUNCTION(BlueprintCallable, Category = "ExWorld|ExWorldTestCharacter|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "ExWorld|ExWorldTestCharacter|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "ExWorld|ExWorldTestCharacter|Attributes")
	float GetLastSpellTime() const;


	UFUNCTION(BlueprintCallable, Category = "ExWorld|ExWorldTestCharacter|Attributes")
	virtual void SetHealth(float Health);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

protected:

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ExWorldTest|Abilities")
	TArray<TSubclassOf<class UExGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ExWorldTest|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	TWeakObjectPtr<class UExAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UExAttributeSetBase> AttributeSetBase;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ExWorld|UI")
	TSubclassOf<UExCharacterStatusBarWidget> UIExCharacterStatusBarClass;

	UPROPERTY()
	UExCharacterStatusBarWidget* UIExCharacterStatusBarWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "ExWorld|UI")
	class UWidgetComponent* UIExCharacterStatusBarComponent;


	virtual void AddCharacterAbilities();
	void InitializeAttributes();



	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ReqSpellAbility();
	void ReqSpellAbility_Implementation();
	bool ReqSpellAbility_Validate();

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ResSpellAbility();

	UFUNCTION()
	void SpellWithCheck();


	UPROPERTY(ReplicatedUsing = OnRep_LastSpellTime)
	int64 LastSpellTime;
	UFUNCTION()
	void OnRep_LastSpellTime();


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


};

