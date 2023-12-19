// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.h"
#include "Floor.generated.h"

class AFloorController;

UCLASS()
class ENDLESSRUNNER_API AFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxComponent;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetFloorController(AFloorController* newController) { FloorController = newController; }

	AFloorController* FloorController;
	
	bool HasOverlapped = false;

	// Obstacles

	void GenerateObstacles();
	void DestroyAllObstacles();

	TArray<FName> SocketNames;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AObstacle>> ObstacleTypes;

	TArray<AObstacle*> SpawnedObstacles;

	int ObstaclesToSpawn = 3;


};
