#include "ExWorldTestPlayerController.h"


#include "AbilitySystemComponent.h"
#include "ExWorldTestPlayerState.h"
#include "ExWorldTestCharacter.h"
#include "UI/ExShowTextWidgetComponent.h"


void AExWorldTestPlayerController::ShowDamageNumber_Implementation(float DamageAmount, AExWorldTestCharacter* TargetCharacter)
{
	if (TargetCharacter)
	{
		UExShowTextWidgetComponent* DamageText = NewObject<UExShowTextWidgetComponent>(TargetCharacter, DamageNumberClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		// DamageText->SetDamageText(DamageAmount);
	}
}

bool AExWorldTestPlayerController::ShowDamageNumber_Validate(float DamageAmount, AExWorldTestCharacter* TargetCharacter)
{
	return true;
}

void AExWorldTestPlayerController::SetRespawnCountdown_Implementation(float RespawnTimeRemaining)
{

}

bool AExWorldTestPlayerController::SetRespawnCountdown_Validate(float RespawnTimeRemaining)
{
	return true;
}

// Server only
void AExWorldTestPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AExWorldTestPlayerState* PS = GetPlayerState<AExWorldTestPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void AExWorldTestPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

}
