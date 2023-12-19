// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

#include "RunnerCharacter.generated.h"



UCLASS()
class ENDLESSRUNNER_API ARunnerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARunnerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

private:

	APlayerController* PlayerController;

	// ===========GamePlay===========
	void InfiniteRun();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Death();
	bool HasDied = false;

	// ==============Score================

	UFUNCTION(BlueprintCallable)
	int GetScore() { return Score; }

	int Score = 0;

	// ============Sliding=============
	// Slide functionality
	void Slide();
	void CalculateSlideTimer(float dT);
	bool CanSlide();
	void BeginSlide();
	void EndSlide();

	UFUNCTION(BlueprintCallable)
	bool GetIsSliding() { return IsSliding; }

	// Slide variables
	float SlideTimer = 0.f;
	bool IsSliding = false;

	// ============Movement=============
	// Move Timeline
	UPROPERTY(EditAnywhere, Category = "Curve")
	UTimelineComponent* MoveTimeline;

	UPROPERTY(EditAnywhere, Category = "Curve")
	UCurveFloat* MoveFloatCurve;

	UFUNCTION()
	void TimelineUpdate(float Value);

	UFUNCTION()
	void TimelineFinished();

	void StartTimeline();

	// Move functionality
	void ChangeLanes(int Direction);
	void MoveLeft() { ChangeLanes(-1); }
	void MoveRight() { ChangeLanes(1); }

	// Move variables
	int CurrentLaneIndex = 1;
	int TargetLaneIndex = CurrentLaneIndex;
	TArray<float> LanePositions = { -325.f, 0.f, 325.f };
	float CurrentYPosition = LanePositions[CurrentLaneIndex];
};
