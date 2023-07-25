
#include "GI.h"

#include "ALevelScorer.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

UGI::UGI()
{
	LoadHiScores();
}

// TODO: Load this from a save file
void UGI::LoadHiScores()
{
	HiScores.Add("Forest", FLevelHiScore(false, 0.0f, 0.0f));
	HiScores.Add("Mountain", FLevelHiScore());
	HiScores.Add("Desert", FLevelHiScore());
	HiScores.Add("Cliffs", FLevelHiScore());
}

FLevelHiScore UGI::GetLevelHiScore(const FString LevelName)
{
	const FLevelHiScore* HiScore = HiScores.Find(LevelName);
	if (HiScore == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find Level in HiScores Map with name: %s"), *LevelName);
		return FLevelHiScore();
	}
	return *HiScore;
}

// TODO: Write to a save file
void UGI::SaveLevelScoreIfHighest(const FString LevelName, const FLevelScore Score)
{
	const FLevelHiScore CurrHiScore = GetLevelHiScore(LevelName);
	const float Acc = Score.ShotsFired == 0 ? 0.0f : static_cast<float>(Score.NumPopped) / Score.ShotsFired * 100;

	if (CurrHiScore.bLocked || Score.TimeElapsed < CurrHiScore.BestTime)
	{
		HiScores.Add(LevelName, FLevelHiScore(false, Score.TimeElapsed, Acc));
		UE_LOG(LogTemp, Warning, TEXT("Added LevelScore for %s"), *LevelName);
	}
}



bool UGI::PlayBGMusic()
{
	if (IsMusicPlaying) return false;

	check(BGMusicWorldA != nullptr);
	UAudioComponent* BGMusicAudioComponent = UGameplayStatics::SpawnSound2D(
		this, BGMusicWorldA, 1, 1, 0,
		nullptr, true, true);
	check(BGMusicAudioComponent != nullptr);
	BGMusicAudioComponent->Play();
	IsMusicPlaying = true;
	
	return true;
}

bool UGI::PlayLobbyMusic()
{
	if (IsMusicPlaying) return false;

	check(LobbyMusic != nullptr);
	UAudioComponent* BGMusicAudioComponent = UGameplayStatics::SpawnSound2D(
		this, LobbyMusic, 1, 1, 0,
		nullptr, false, true);
	check(BGMusicAudioComponent != nullptr);
	BGMusicAudioComponent->Play();
	// IsMusicPlaying = true;
	
	return true;
}

void UGI::PlayFailSound()
{
	check(FailSound != nullptr);
	UAudioComponent* FailSoundAudioComponent = UGameplayStatics::SpawnSound2D(
		this, FailSound, 1, 1, 0,
		nullptr, true, true);
	check(FailSoundAudioComponent != nullptr);
	FailSoundAudioComponent->Play();
}

void UGI::PlayDingSound()
{
	check(DingSound != nullptr);
	UAudioComponent* DingSoundAudioComponent = UGameplayStatics::SpawnSound2D(
		this, DingSound, 1, 1, 0,
		nullptr, true, true);
	check (DingSoundAudioComponent != nullptr);
	DingSoundAudioComponent->Play();
}

void UGI::PlayShootSound()
{
	check(ShootSound != nullptr);
	UAudioComponent* ShootSoundAudioComponent = UGameplayStatics::SpawnSound2D(
		this, ShootSound, 1, 1, 0,
		nullptr, true, true);
	check (ShootSoundAudioComponent != nullptr);
	ShootSoundAudioComponent->Play();
}
