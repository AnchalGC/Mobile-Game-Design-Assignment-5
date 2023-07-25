
#include "Balloon.h"

// Sets default values for this component's properties
ABalloon::ABalloon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetNotifyRigidBodyCollision(true);
	RootComponent = Mesh;

	if (static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(SpherePath); SphereMesh.Succeeded())
	{
		Mesh->SetStaticMesh(SphereMesh.Object);
		Mesh->SetWorldScale3D(FVector(0.3f));
	}

	if (static ConstructorHelpers::FObjectFinder<UMaterial> RedMat(RedMatPath); RedMat.Succeeded())
	{
		EColorToMatInst.Add(GameLogic::EColor::Red, UMaterialInstanceDynamic::Create(RedMat.Object, Mesh));
	} else UE_LOG(LogTemp, Error, TEXT("Could not load Red material!"));

	if (static ConstructorHelpers::FObjectFinder<UMaterial> GreenMat(GreenMatPath); GreenMat.Succeeded())
	{
		EColorToMatInst.Add(GameLogic::EColor::Green, UMaterialInstanceDynamic::Create(GreenMat.Object, Mesh));
	} else UE_LOG(LogTemp, Error, TEXT("Could not load Green material!"));

	if (static ConstructorHelpers::FObjectFinder<UMaterial> ErrMat(ErrMatPath); ErrMat.Succeeded())
	{
		EColorToMatInst.Add(GameLogic::EColor::None, UMaterialInstanceDynamic::Create(ErrMat.Object, Mesh));
	} else UE_LOG(LogTemp, Error, TEXT("Could not load Err material!"));
}


// Called when the game starts
void ABalloon::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ABalloon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ...
}

GameLogic::EColor ABalloon::GetColor() const
{
	return Color;
}


void ABalloon::SetColor(const GameLogic::EColor InColor)
{
	Color = InColor;
	Mesh->SetMaterial(0, *EColorToMatInst.Find(InColor));
}



