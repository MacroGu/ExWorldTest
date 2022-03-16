#include "ExCharacterStatusBarWidget.h"

#include "Components/ProgressBar.h"




void UExCharacterStatusBarWidget::SetHealthPercentage(float HealthPercentage)
{
	ProgressBar_Health->SetPercent(HealthPercentage);
}

void UExCharacterStatusBarWidget::SetManaPercentage(float ManaPercentage)
{
	ProgressBar_Mana->SetPercent(ManaPercentage);
}

