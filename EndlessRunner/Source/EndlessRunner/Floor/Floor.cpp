// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "FloorController.h"


// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	FTransform BoxTransform;
	FVector Loc = FVector(1050.f, 0.f, 160.f);
	FVector Scale = FVector(2.f, 15.f, 5.f);
	BoxTransform.SetLocation(Loc);
	BoxTransform.SetScale3D(Scale);


	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionProfileName("No Collision");
	BoxComponent->AddLocalTransform(BoxTransform);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFloor::OnOverlapBegin);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Core/Meshes/FloorTile.FloorTile'"));
	if (MeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshAsset.Object);
	}

	SocketNames.Add(FName("LeftLaneSlot"));
	SocketNames.Add(FName("MiddleLaneSlot"));
	SocketNames.Add(FName("RightLaneSlot"));

	
}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateObstacles();
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AFloor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasOverlapped)
	{
		HasOverlapped = true;

		if (OtherActor)
		{
			if (FloorController)
			{
				FloorController->DestroyFloor();
				
			}
			
		}
	}
	

}

void AFloor::GenerateObstacles()
{
	TArray<TSubclassOf<AObstacle>> localObs = ObstacleTypes;
	TArray<FName> localPositions = SocketNames;

	int obstacleIndex = 0;
	int positionIndex = 0;

	AObstacle* newObstacle;

	for (int i = 0; i < ObstaclesToSpawn; i++)
	{
		obstacleIndex = FMath::RandRange(0, localObs.Num() - 1);
		positionIndex = FMath::RandRange(0, localPositions.Num() - 1);

		newObstacle = GetWorld()->SpawnActor<AObstacle>(localObs[obstacleIndex]);
		newObstacle->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale, localPositions[positionIndex]);

		SpawnedObstacles.Add(newObstacle);

		localObs.RemoveAt(obstacleIndex);
		localPositions.RemoveAt(positionIndex);
	}


}

void AFloor::DestroyAllObstacles()
{
	int ArrayLength = SpawnedObstacles.Num();

	for (int i = 0; i < ArrayLength; i++)
	{	
		SpawnedObstacles[i]->Destroy();
	}
}

