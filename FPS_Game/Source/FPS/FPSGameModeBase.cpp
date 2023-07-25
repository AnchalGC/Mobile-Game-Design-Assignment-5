// Copyright Epic Games, Inc. All Rights Reserved.


#include "FPSGameModeBase.h"

#include "GI.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AFPSGameModeBase::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);
	// GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Yellow, TEXT("This is FPSGameMode."));
}

void AFPSGameModeBase::DisplayLevelEndWidget(const FLevelScore& Score, bool bPlayerMatchColor)
{
	if (UGI* GameInst = Cast<UGI>(GetGameInstance()); GameInst != nullptr)
	{
		if (bPlayerMatchColor)
		{
			GameInst->SaveLevelScoreIfHighest(UGameplayStatics::GetCurrentLevelName(this), Score);
			UE_LOG(LogTemp, Display, TEXT("Score was submitted for saving."));
		}
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get GameInstance in DisplayLevelEndWidget"));
	}
	UE_LOG(LogTemp, Warning, TEXT("bPlayerMatchColor: %s"), (bPlayerMatchColor ? TEXT("true") : TEXT("false")));
	const float Accuracy = Score.ShotsFired == 0 ? 0.0f : static_cast<float>(Score.NumPopped) / Score.ShotsFired * 100;
	ULevelEndWidget* NewWidget = CreateWidget<ULevelEndWidget>(GetWorld(), LevelEndClass);
	
	const FLevelReqs LevelReqs = GetLevelReqs(UGameplayStatics::GetCurrentLevelName(this));
	NewWidget->PassedBalloons = Score.NumPopped == Score.TotalPoppable;
	NewWidget->PassedColor = bPlayerMatchColor;
	NewWidget->PassedTime = Score.TimeElapsed <= LevelReqs.Time;
	NewWidget->PassedAccuracy = Accuracy >= LevelReqs.Accuracy;

	NewWidget->AllowNextLevel =
		(NewWidget->PassedBalloons && NewWidget->PassedColor && NewWidget->PassedTime && NewWidget->PassedAccuracy);
	
	NewWidget->TextBalloons = FString::Printf(TEXT("Balloons: %d/%d"), Score.NumPopped, Score.TotalPoppable);
	NewWidget->TextTime = FString::Printf(TEXT("Time: %.1fs"), Score.TimeElapsed);
	NewWidget->TextColor = FString::Printf(TEXT("Color"));
	NewWidget->TextAccuracy = FString::Printf(TEXT("Accuracy: %.1f%%"), Accuracy);
	NewWidget->AddToViewport();

	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	Controller->SetInputMode(FInputModeUIOnly());
	NewWidget->SetFocus();
}

FName AFPSGameModeBase::GetNextLevel(const FString& CurrLevel)
{
	if (CurrLevel == "Sandbox") return "Sandbox2";
	if (CurrLevel == "Sandbox2") return "Forest";
	if (CurrLevel == "Forest") return "Mountain";
	if (CurrLevel == "Mountain") return "Lobby";
	UE_LOG(LogTemp, Error, TEXT("UNKNOWN LEVEL NAME: %s"), *CurrLevel);
	return "UNKNOWN";
}


FLevelReqs AFPSGameModeBase::GetLevelReqs(const FString& LevelName)
{
	if (LevelName == "Sandbox") return FLevelReqs(100.f, 10.f);
	if (LevelName == "Sandbox2") return FLevelReqs(100.f, 7.5f);
	if (LevelName == "Forest") return FLevelReqs(90.f, 16.5f);
	if (LevelName == "Mountain") return FLevelReqs(100.f, 65.f);

	UE_LOG(LogTemp, Error, TEXT("Missing GetLevelReqs case for LevelName: %s"), *LevelName);
	return FLevelReqs(-1.f, -1.f);
}
