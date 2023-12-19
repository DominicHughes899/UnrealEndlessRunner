// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Floor.h"

#include "FloorController.generated.h"

UCLASS()
class ENDLESSRUNNER_API AFloorController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TellPlayerController();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AFloor> FloorActor;

	void ExtendFloor();

	void AddFirstFloor();
	void AddFloor();
	void DestroyFloor();

	TArray<AFloor*> FloorArray;

	AFloor* LastFloorSpawned;
	AFloor* FirstFloor;

	int MaxNumberOfFloors = 20;

};
