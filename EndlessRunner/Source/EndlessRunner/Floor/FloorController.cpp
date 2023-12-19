// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorController.h"


// Sets default values
AFloorController::AFloorController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AFloorController::BeginPlay()
{
	Super::BeginPlay();

	AddFirstFloor();

}

// Called every frame
void AFloorController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

void AFloorController::TellPlayerController()
{
	
}


void AFloorController::ExtendFloor()
{
	AddFloor();

	if (FloorArray.Num() > MaxNumberOfFloors)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, "ShouldDelete");
		FloorArray[0]->DestroyAllObstacles();
		FloorArray[0]->Destroy();
		FloorArray.RemoveAt(0);
	}

}

void AFloorController::AddFirstFloor()
{
	if (FloorActor)
	{
		LastFloorSpawned = GetWorld()->SpawnActor<AFloor>(FloorActor, GetActorLocation(), GetActorRotation());

		FloorArray.Add(LastFloorSpawned);
		LastFloorSpawned->SetFloorController(this);

		for (int i = 0; i < MaxNumberOfFloors - 2; i++)
		{
			AddFloor();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Floor Actor not found");
	}
	
}

void AFloorController::AddFloor()
{
	if (FloorActor)
	{
		AFloor* newFloor = GetWorld()->SpawnActor<AFloor>(FloorActor, GetActorLocation(), GetActorRotation());

		newFloor->AttachToActor(LastFloorSpawned, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("AttachPoint"));
		newFloor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		newFloor->SetFloorController(this);

		FloorArray.Add(newFloor);

		LastFloorSpawned = newFloor;
	}
	
}

void AFloorController::DestroyFloor()
{
	ExtendFloor();
}


