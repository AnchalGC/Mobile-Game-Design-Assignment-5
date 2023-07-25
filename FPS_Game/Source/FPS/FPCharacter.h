
#pragma once

#include "CoreMinimal.h"
#include "Shot.h"
#include "GameFramework/Character.h"
#include "FPCharacter.generated.h"


UCLASS()
class FPS_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCharacter();

	DECLARE_EVENT(AFPCharacter, FShotFiredEvent);
	FShotFiredEvent OnShotFired;

protected:
	////  REFS  ////
	class UGI* GI;
	
	////  PROPERTIES  ////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Color Logic")
	UMaterial* GreenMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Color Logic")
	UMaterial* RedMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Color Logic")
	UMaterial* BlueMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Color Logic")
	UMaterial* WhiteMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color Logic")
	UMaterial* ErrMat;
	
	
	////  STRUCTURE  ////
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FPCamera;

	// This is for the arms-n-hands mesh visible only to player
	// UPROPERTY(VisibleAnywhere)
	// USkeletalMeshComponent* FPMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* GunNiagaraParticles;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* ProjectileSpawn;
	//////////////////////////////////////////////////////////////////////

	
	//// PROJECTILES  ////
	UPROPERTY(EditAnywhere, Category="Projectiles")
	TSubclassOf<ABaseProjectile> ClassShot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectiles")
	TEnumAsByte<GameLogic::EColor> ProjectileColor = GameLogic::EColor::Green;
	
	// If no charge
	UPROPERTY(EditAnywhere, Category="Projectiles")
	float ShotMaxSpeed = 10000.f;

	// If max charge
	UPROPERTY(EditAnywhere, Category="Projectiles")
	float ShotMinSpeed = 1000.f;
	float ShotCurrCharge = ShotMinSpeed;
	
	UPROPERTY(EditAnywhere, Category="Projectiles")
	float ShotFullChargeSeconds = 2.f;
	float ShotChargeRate = (ShotMaxSpeed - ShotMinSpeed) / ShotFullChargeSeconds;
	bool bCharging = false;
	float MuzzleRotateAccel = 50.f;
	float MuzzleCurrRotatingSpeed = 0.f;
	float MuzzleMaxRotateSpeed = ShotFullChargeSeconds * MuzzleRotateAccel;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateGunMeshColor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetForwardMovement(const float Amount);
	void SetStrafeMovement(const float Amount);
	

	// Actions
	void ReleaseShot();
	void StartShotCharge();

	UFUNCTION(BlueprintCallable)
	void SetProjectileColor(GameLogic::EColor InColor);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHUD(const FString &PoppedProgress, const FString &LevelName);
};
