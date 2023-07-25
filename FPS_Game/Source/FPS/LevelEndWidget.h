
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelEndWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API ULevelEndWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	bool AllowNextLevel;
	
	UPROPERTY(BlueprintReadOnly)
	FName NextLevelName;



	UPROPERTY(BlueprintReadOnly)
	bool PassedBalloons;

	UPROPERTY(BlueprintReadOnly)
	FString TextBalloons;

	UPROPERTY(BlueprintReadOnly)
	bool PassedTime;

	UPROPERTY(BlueprintReadOnly)
	FString TextTime;

	UPROPERTY(BlueprintReadOnly)
	bool PassedColor;

	UPROPERTY(BlueprintReadOnly)
	FString TextColor;

	UPROPERTY(BlueprintReadOnly)
	bool PassedAccuracy;

	UPROPERTY(BlueprintReadOnly)
	FString TextAccuracy;
};
