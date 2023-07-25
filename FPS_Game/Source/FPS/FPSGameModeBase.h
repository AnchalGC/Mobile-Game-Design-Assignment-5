// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ALevelScorer.h"
#include "LevelEndWidget.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameModeBase.generated.h"


USTRUCT(BlueprintType)
struct FLevelReqs
{
	GENERATED_BODY()
	float Accuracy;
	float Time;

	FLevelReqs() { Accuracy = -1.f; Time = -1.f; }
	FLevelReqs(const float &InAccuracy, const float &InTime) { Accuracy = InAccuracy, Time = InTime; }
};

UCLASS()
class FPS_API AFPSGameModeBase : public AGameModeBase
{

	GENERATED_BODY()

	virtual void StartPlay() override;

public:

	
	UFUNCTION(BlueprintCallable)
	void DisplayLevelEndWidget(const FLevelScore &Score, bool bPlayerMatchColor);

	UFUNCTION(BlueprintCallable)
	FName GetNextLevel(const FString &CurrLevel);
	
	UFUNCTION(BlueprintCallable)
	static FLevelReqs GetLevelReqs(const FString &LevelName);

protected:
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	// TSubclassOf<UUserWidget> LevelEndWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (BlueprintProtected = "true"))
	TSubclassOf<ULevelEndWidget> LevelEndClass;
};