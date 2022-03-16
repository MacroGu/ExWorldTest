
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExCharacterStatusBarWidget.generated.h"


class UProgressBar;



/**
 * 
 */
UCLASS()
class EXWORLDTEST_API UExCharacterStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetHealthPercentage(float HealthPercentage);

	UFUNCTION(BlueprintCallable)
	void SetManaPercentage(float ManaPercentage);


public:
	UPROPERTY(Meta = (BindWidget))
	UProgressBar* ProgressBar_Health = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UProgressBar* ProgressBar_Mana = nullptr;

};
