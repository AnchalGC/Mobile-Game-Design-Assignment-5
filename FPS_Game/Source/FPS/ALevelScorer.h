
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ALevelScorer.generated.h"

USTRUCT(BlueprintType)
struct FLevelScore
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float TimeElapsed;

	UPROPERTY(BlueprintReadOnly)
	int ShotsFired;

	UPROPERTY(BlueprintReadOnly)
	int NumPopped;

	UPROPERTY(BlueprintReadOnly)
	int TotalPoppable;

	FLevelScore() { TimeElapsed = -1.f; ShotsFired = -1.f; NumPopped = -1.f; TotalPoppable = -1.f;}
};

UCLASS()
class FPS_API ALevelScorer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelScorer();

	UFUNCTION(BlueprintCallable)
	FLevelScore GetScore() const;

	UFUNCTION()
	void IncrementShotsFired();
	
	UFUNCTION()
	void IncrementNumPopped();

	static FLevelScore CompareScores(FString LevelName, FLevelScore ScoreA, FLevelScore ScoreB);

protected:
	DECLARE_DELEGATE(FIncrementShotsFiredDelegate);
	DECLARE_DELEGATE(FIncrementNumPoppedDelegate);
	
	class AFPCharacter* Player;
	
	float TimeElapsed = 0.f;
	int ShotsFired = 0;
	int NumPopped = 0;
	int TotalPoppable = 0;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerHUD();

};
