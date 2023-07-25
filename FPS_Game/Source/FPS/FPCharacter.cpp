
#include "FPCharacter.h"

#include "Camera/CameraComponent.h"
#include "GI.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AFPCharacter::AFPCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check(FPCamera != nullptr);
	FPCamera->SetupAttachment(GetRootComponent());
	FPCamera->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight));
	FPCamera->bUsePawnControlRotation = true;

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	GunMesh->SetupAttachment(FPCamera);


	GetMesh()->SetOwnerNoSee(true);

	ProjectileSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawn"));
	ProjectileSpawn->SetupAttachment(FPCamera);
}

// Called when the game starts or when spawned
void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "FPCharacter BeginPlay");

	GI = Cast<UGI>(GetGameInstance());
	check(GI != nullptr);

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	check(PlayerController != nullptr);
	PlayerController->SetInputMode(FInputModeGameOnly());
}

// Called every frame
void AFPCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Movement.Normalize();
	// AddActorLocalOffset(Movement * MoveSpeed);

	check(GEngine != nullptr);
	// const float Length = (Movement * MoveSpeed).Length();
	// GEngine->AddOnScreenDebugMessage(23, 1.0f, FColor::Emerald,
	//                                  FString::Printf(TEXT("Speed: %.3f"), GetVelocity().Length()));

	if (bCharging)
	{
		ShotCurrCharge = FMath::Min(ShotMaxSpeed, ShotCurrCharge + ShotChargeRate * DeltaTime);
	}
}

// Called to bind functionality to input
void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AFPCharacter::SetForwardMovement);
	PlayerInputComponent->BindAxis(TEXT("StrafeRight"), this, &AFPCharacter::SetStrafeMovement);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AFPCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AFPCharacter::AddControllerYawInput);

	// Actions
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AFPCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AFPCharacter::StartShotCharge);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &AFPCharacter::ReleaseShot);
}

void AFPCharacter::SetForwardMovement(const float Amount)
{
	// Movement.X = Amount;
	AddMovementInput(GetActorForwardVector(), Amount);
}

void AFPCharacter::SetStrafeMovement(const float Amount)
{
	// Movement.Y = Amount;
	AddMovementInput(GetActorRightVector(), Amount);
}

void AFPCharacter::StartShotCharge()
{
	ShotCurrCharge = ShotMinSpeed;
	bCharging = true;
}

void AFPCharacter::ReleaseShot()
{
	// Play sound first to be as responsive as possible
	GI->PlayShootSound();
	
	const FTransform Transform = FTransform(ProjectileSpawn->GetComponentRotation(),
	                                        ProjectileSpawn->GetComponentLocation());
	ABaseProjectile* SpawnedShot = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ClassShot, Transform, this);
	check(SpawnedShot != nullptr);
	SpawnedShot->SetInitAndMaxTravelSpeed(ShotCurrCharge);
	SpawnedShot->SetGravity(0.f);
	SpawnedShot->SetInstigator(GetInstigator());
	SpawnedShot->SetColor(ProjectileColor);
	SpawnedShot->FinishSpawning(Transform);

	UE_LOG(LogTemp, Display, TEXT("Fired %s shot with speed: %.1f"),
	       *UConstants::EColorToString(SpawnedShot->GetColor()), ShotCurrCharge);

	ShotCurrCharge = ShotMinSpeed;
	bCharging = false;

	OnShotFired.Broadcast();

	// UGameplayStatics::SpawnEmitterAtLocation(this, GunParticles, ProjectileSpawn->GetComponentLocation());
	// UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, GunNiagaraParticles, ProjectileSpawn->GetComponentLocation());
	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		GunNiagaraParticles, GunMesh, TEXT("SocketMuzzleFlash"),
		GunMesh->GetSocketLocation(TEXT("SocketMuzzleFlash")),
		GunMesh->GetComponentRotation(),
		EAttachLocation::Type::KeepWorldPosition,
		true);

	FLinearColor ParticleColor = UConstants::EColorToFLinearColor(ProjectileColor);
	NiagaraComponent->SetNiagaraVariableLinearColor(FString("ParticleColor"), ParticleColor);
}

void AFPCharacter::SetProjectileColor(const GameLogic::EColor InColor)
{
	ProjectileColor = InColor;
	UpdateGunMeshColor();
	UE_LOG(LogTemp, Display, TEXT("Changed ProjectilColor to %s"), *UConstants::EColorToString(ProjectileColor));
}