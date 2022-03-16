// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExWorldTestCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


#include "UI/ExCharacterStatusBarWidget.h"
#include "ExWorldTestPlayerController.h"
#include "Abilities/AttributeSets/ExAttributeSetBase.h"
#include "ExWorldTestGameMode.h"
#include "ExWorldTestPlayerState.h"



//////////////////////////////////////////////////////////////////////////
// AExWorldTestCharacter

AExWorldTestCharacter::AExWorldTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	UIExCharacterStatusBarComponent = CreateDefaultSubobject<UWidgetComponent>(FName("UIExCharacterStatusBarComponent"));
	UIExCharacterStatusBarComponent->SetupAttachment(RootComponent);
	UIExCharacterStatusBarComponent->SetRelativeLocation(FVector(0, 0, 120));
	UIExCharacterStatusBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	UIExCharacterStatusBarComponent->SetDrawSize(FVector2D(500, 500));

	if (!UIExCharacterStatusBarClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() UIExCharacterStatusBarClass should be set"), *FString(__FUNCTION__));
	}

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));

}

void AExWorldTestCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AExWorldTestPlayerState* PS = GetPlayerState<AExWorldTestPlayerState>();
	if (!PS)
	{
		return;
	}

	AbilitySystemComponent = Cast<UExAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

	AttributeSetBase = PS->GetAttributeSetBase();

	InitializeAttributes();
	InitializeCharacterStatusBar();
	AddCharacterAbilities();

}

void AExWorldTestCharacter::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UExGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

void AExWorldTestCharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 0, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}

}

void AExWorldTestCharacter::UnPossessed()
{

}

void AExWorldTestCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Our controller changed, must update ActorInfo on AbilitySystemComponent
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

void AExWorldTestCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

UAbilitySystemComponent* AExWorldTestCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AExWorldTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AExWorldTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AExWorldTestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AExWorldTestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AExWorldTestCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AExWorldTestCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AExWorldTestCharacter::TouchStopped);

	BindASCInput();
}

void AExWorldTestCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AExWorldTestCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AExWorldTestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AExWorldTestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void AExWorldTestCharacter::ChangeHealth_Implementation(float NewValue)
{
	if (!AttributeSetBase.IsValid() || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	AbilitySystemComponent->ApplyModToAttributeUnsafe(AttributeSetBase->GetHealthAttribute(), EGameplayModOp::Additive, NewValue);

}

bool AExWorldTestCharacter::ChangeHealth_Validate(float NewValue)
{
	return true;
}

void AExWorldTestCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AExWorldTestCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AExWorldTestCharacter::InitializeCharacterStatusBar()
{
	// Only create once
	if (UIExCharacterStatusBarWidget || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	// Setup UI for Locally Owned Players only, not AI or the server's copy of the PlayerControllers
	AExWorldTestPlayerController* PC = Cast<AExWorldTestPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC || !PC->IsLocalPlayerController())
	{
		return;
	}

	if (!UIExCharacterStatusBarClass)
	{
		return;
	}

	UIExCharacterStatusBarWidget = CreateWidget<UExCharacterStatusBarWidget>(PC, UIExCharacterStatusBarClass);
	if (!UIExCharacterStatusBarWidget || !UIExCharacterStatusBarComponent)
	{
		return;
	}

	UIExCharacterStatusBarComponent->SetWidget(UIExCharacterStatusBarWidget);

	// Setup the floating status bar
	UIExCharacterStatusBarWidget->SetHealthPercentage(GetMaxHealth() / GetMaxHealth());

}

void AExWorldTestCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AExWorldTestPlayerState* PS = GetPlayerState<AExWorldTestPlayerState>();
	if (!PS)
	{
		return;
	}

	AbilitySystemComponent = Cast<UExAbilitySystemComponent>(PS->GetAbilitySystemComponent());

	AbilitySystemComponent->InitAbilityActorInfo(PS, this);

	BindASCInput();

	AttributeSetBase = PS->GetAttributeSetBase();

	// InitializeAttributes();

	InitializeCharacterStatusBar();


	AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

	SetHealth(GetMaxHealth());

}

void AExWorldTestCharacter::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString(TEXT("ConfirmTarget")), FString("CancelTarget"), FString("EAbilityInputID")));

		ASCInputBound = true;
	}
}


float AExWorldTestCharacter::GetHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetHealth();
	}

	return 0.0f;
}

float AExWorldTestCharacter::GetMaxHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxHealth();
	}

	return 0.0f;
}

void AExWorldTestCharacter::SetHealth(float Health)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetHealth(Health);
	}
}
