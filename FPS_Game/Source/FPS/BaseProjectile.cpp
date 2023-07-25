
#include "BaseProjectile.h"

#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetNotifyRigidBodyCollision(true);
	RootComponent = Mesh;

	TrailParticles = CreateDefaultSubobject<UNiagaraComponent>("TrailParticles");
	TrailParticles->SetupAttachment(RootComponent);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);

	StartLoc = GetActorLocation();
	UE_LOG(LogTemp, Display, TEXT("BeginPlay finished"));
	
}

// Called every frame
void ABaseProjectile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if-init syntax cooL!!!
	if (const float TravelDist = (GetActorLocation() - StartLoc).Length(); TravelDist >= DestroyAfterTravelDist)
	{
		Destroy();
		UE_LOG(LogTemp, Display, TEXT("%s destroyed after travelling %.1f"), *GetActorNameOrLabel(), TravelDist);
	}
}

void ABaseProjectile::OnHit(
	UPrimitiveComponent* Hitter,
	AActor* StruckActor,
	UPrimitiveComponent* StruckComp,
	FVector NormalImpulse,
	const FHitResult &HitResult)
{
	UE_LOG(LogTemp, Display, TEXT("OnHit from BaseProjectile. Struck: %s"), *StruckActor->GetActorNameOrLabel());

	// Splatter
	Mesh->SetVisibility(false);
	//Destroy();
}

float ABaseProjectile::SetInitAndMaxTravelSpeed(const float Speed)
{
	Movement->MaxSpeed = Speed;
	Movement->InitialSpeed = Speed;
	return Speed;
}

float ABaseProjectile::SetGravity(float Gravity)
{
	Movement->ProjectileGravityScale = Gravity;
	return Gravity;
}

GameLogic::EColor ABaseProjectile::GetColor() const
{
	return this->Color;
}

void ABaseProjectile::SetColor(const GameLogic::EColor InColor)
{
	this->Color = InColor;
	UE_LOG(LogTemp, Display, TEXT("SetColor called in BaseProjectile"));
}
