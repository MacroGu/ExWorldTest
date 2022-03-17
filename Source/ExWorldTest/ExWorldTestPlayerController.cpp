#include "ExWorldTestPlayerController.h"


#include "AbilitySystemComponent.h"
#include "ExWorldTestPlayerState.h"
#include "ExWorldTestCharacter.h"
#include "UI/ExTimeLimitationShow.h"



AExWorldTestPlayerController::AExWorldTestPlayerController()
{
}

void AExWorldTestPlayerController::ShowTimeLimitation(const int64 TimeLeft)
{
	UITimeLimitation = CreateWidget<UExTimeLimitationShow>(this, TimeLimitationClass);
	UITimeLimitation->AddToViewport();
	UITimeLimitation->UpdateTImeLeft(TimeLeft);
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
