
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExTimeLimitationShow.generated.h"


/**
 * 
 */
UCLASS()
class EXWORLDTEST_API UExTimeLimitationShow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTImeLeft(int64 TimeLeft);

};
