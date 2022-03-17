#include "ExWorldTestPlayerState.h"

#include "Abilities/AttributeSets/ExAttributeSetBase.h"
#include "ExWorldTestCharacter.h"
#include "ExWorldTestPlayerController.h"
#include "UI/ExCharacterStatusBarWidget.h"
#include "Abilities/ExAbilitySystemComponent.h"
#include "Abilities/AttributeSets/ExAttributeSetBase.h"


AExWorldTestPlayerState::AExWorldTestPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UExAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another ExWorldTestPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	AttributeSetBase = CreateDefaultSubobject<UExAttributeSetBase>(TEXT("AttributeSetBase"));

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
	NetUpdateFrequency = 100.0f;

	// Cache tags
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent * AExWorldTestPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UExAttributeSetBase* AExWorldTestPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool AExWorldTestPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void AExWorldTestPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{

}

float AExWorldTestPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float AExWorldTestPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

void AExWorldTestPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	// Attribute change callbacks
	HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AExWorldTestPlayerState::HealthChanged);
	MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &AExWorldTestPlayerState::MaxHealthChanged);

}

void AExWorldTestPlayerState::HealthChanged(const FOnAttributeChangeData & Data)
{
	float Health = Data.NewValue;

	// Update floating status bar
	AExWorldTestCharacter* Hero = Cast<AExWorldTestCharacter>(GetPawn());
	if (!IsValid(Hero))
	{
		return;
	}

	UExCharacterStatusBarWidget* HeroFloatingStatusBar = Hero->GetExPlayerStatusBar();
	if (HeroFloatingStatusBar)
	{
		HeroFloatingStatusBar->SetHealthPercentage(Health / GetMaxHealth());
	}

}

void AExWorldTestPlayerState::MaxHealthChanged(const FOnAttributeChangeData & Data)
{
	float MaxHealth = Data.NewValue;

	// Update floating status bar
	AExWorldTestCharacter* Hero = Cast<AExWorldTestCharacter>(GetPawn());
	if (!IsValid(Hero))
	{
		return;
	}

	UExCharacterStatusBarWidget* HeroFloatingStatusBar = Hero->GetExPlayerStatusBar();
	if (HeroFloatingStatusBar)
	{
		HeroFloatingStatusBar->SetHealthPercentage(GetHealth() / MaxHealth);
	}

}
