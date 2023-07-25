
#include "Shot.h"
#include "Constants.h"


AShot::AShot() { }

void AShot::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(UConstants::TagShot);
	Tags.Add(UConstants::TagPoppable);
	
	UE_LOG(LogTemp, Display, TEXT("BeginPlay finished."));
}

void AShot::OnHit(
	UPrimitiveComponent* Hitter,
	AActor* StruckActor,
	UPrimitiveComponent* StruckComp,
	FVector NormalImpulse,
	const FHitResult &HitResult)
{
	Super::OnHit(Hitter, StruckActor, StruckComp, NormalImpulse, HitResult);
	UE_LOG(LogTemp, Display, TEXT("AShot OnHit: %s hit %s"), *GetActorNameOrLabel(), *StruckComp->GetName());
}

// void AShot::SetColor(const GameLogic::EColor InColor)
// {
// 	Super::SetColor(InColor);
// 	Mesh->SetMaterial(0, *EColorToMatInst.Find(InColor));
// }