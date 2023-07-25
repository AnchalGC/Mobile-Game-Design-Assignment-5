
#pragma once

#include "CoreMinimal.h"
#include "Constants.h"
#include "GameFramework/Actor.h"
#include "TwoToneFloater.generated.h"

UCLASS()
class FPS_API ATwoToneFloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATwoToneFloater();

	DECLARE_DELEGATE(FCallTestBindDelegate)

protected:
	bool AlreadyStruck = false;

	////////////// REFS //////////////
	class AFPCharacter* Player;
	class UGI* GI;
	class ALevelScorer* LevelScorer;


	////////////// PROPERTIES //////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Color Logic")
	UMaterial* GreenMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Color Logic")
	UMaterial* RedMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Color Logic")
	UMaterial* BlueMat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Color Logic")
	UMaterial* WhiteMat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Color Logic")
	UMaterial* ErrMat;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* ConfettiParticles;
	
	
	////////////// STRUCTURE //////////////
	// Having a bare scene component as root allows MeshA and MeshB to be siblings, each a child
	// of this root. Neither Mesh has the additional responsibility of being the primary transform.
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Scene;
	
	// MeshA and MeshB should each have a color associated.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Color Logic")
	TEnumAsByte<GameLogic::EColor> ColorA;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Color Logic")
	TEnumAsByte<GameLogic::EColor> ColorB;
	///////////////////////////////////////


	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& HitResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
