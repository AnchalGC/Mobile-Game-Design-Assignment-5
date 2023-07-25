
#include "ALevelScorer.h"

#include "FPSGameModeBase.h"
#include "FPCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelScorer::ALevelScorer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALevelScorer::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AFPCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	check(Player != nullptr);

	FIncrementShotsFiredDelegate ShotsDelegate;
	ShotsDelegate.BindUObject(this, &ALevelScorer::IncrementShotsFired);
	Player->OnShotFired.Add(ShotsDelegate);

	FIncrementNumPoppedDelegate NumPoppedDelegate;
	NumPoppedDelegate.BindUObject(this, &ALevelScorer::IncrementNumPopped);


	TArray<AActor*> Poppables;
	UGameplayStatics::GetAllActorsWithTag(this, UConstants::TagPoppable, Poppables);
	TotalPoppable = Poppables.Num();

	UE_LOG(LogTemp, Display, TEXT("Init TotalPoppable to: %d"), TotalPoppable);

	TimeElapsed = 0.f;
	UpdatePlayerHUD();
}

void ALevelScorer::IncrementShotsFired()
{
	ShotsFired++;
	UE_LOG(LogTemp, Display, TEXT("Incremented ShotsFired"));
}

void ALevelScorer::IncrementNumPopped()
{
	NumPopped++;
	UE_LOG(LogTemp, Display, TEXT("Incremented NumPopped"));
	UpdatePlayerHUD();
}


FLevelScore ALevelScorer::GetScore() const
{
	FLevelScore Score;
	Score.TimeElapsed = TimeElapsed;
	Score.ShotsFired = ShotsFired;
	Score.TotalPoppable = TotalPoppable;
	Score.NumPopped = NumPopped;

	return Score;
}

// Called every frame
void ALevelScorer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeElapsed += DeltaTime;
}

void ALevelScorer::UpdatePlayerHUD()
{
	const FString Progress = FString::FromInt(NumPopped) + "/" + FString::FromInt(TotalPoppable);
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	Player->UpdateHUD(Progress, LevelName);
}


FLevelScore ALevelScorer::CompareScores(const FString LevelName, FLevelScore ScoreA, FLevelScore ScoreB)
{
	const FLevelReqs PassingScore = AFPSGameModeBase::GetLevelReqs(LevelName);
	const float AccA = ScoreA.ShotsFired == 0 ? 0.0f : static_cast<float>(ScoreA.NumPopped) / ScoreA.ShotsFired * 100;
	const float AccB = ScoreB.ShotsFired == 0 ? 0.0f : static_cast<float>(ScoreB.NumPopped) / ScoreB.ShotsFired * 100;

	const bool ScoreAPassed =
		(ScoreA.NumPopped == ScoreA.TotalPoppable)
			&& ScoreA.TimeElapsed <= PassingScore.Time
			&& AccA >= PassingScore.Accuracy;

	const bool ScoreBPassed =
		(ScoreB.NumPopped == ScoreB.TotalPoppable)
			&& ScoreB.TimeElapsed <= PassingScore.Time
			&& AccB >= PassingScore.Accuracy;

	if (ScoreAPassed && !ScoreBPassed) return ScoreA;
	if (ScoreBPassed && !ScoreAPassed) return ScoreB;

	if (ScoreA.TimeElapsed < ScoreB.TimeElapsed) return ScoreA;
	if (ScoreB.TimeElapsed > ScoreA.TimeElapsed) return ScoreB;
	if (AccA > AccB) return ScoreA;
	return ScoreB;
}
