// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ARunnerCharacter::ARunnerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Construct Curve
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder(TEXT("CurveFloat'/Game/Core/Blueprints/Curves/C_ChangeLane.C_ChangeLane'"));
	if (CurveFinder.Succeeded())
	{
		MoveFloatCurve = CurveFinder.Object;
	}

	// Construct Timeline
	MoveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveTimeline"));
	
	// Bind functions to timeline events
	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("TimelineUpdate"));
	MoveTimeline->AddInterpFloat(MoveFloatCurve, TimelineProgress);

	FOnTimelineEvent TimelineFinishedEvent;
	TimelineFinishedEvent.BindUFunction(this, FName("TimelineFinished"));
	MoveTimeline->SetTimelineFinishedFunc(TimelineFinishedEvent);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARunnerCharacter::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	
}

// Called every frame
void ARunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InfiniteRun();

	CalculateSlideTimer(DeltaTime);

}

// Called to bind functionality to input
void ARunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("IA_Jump", IE_Pressed, this, &ARunnerCharacter::Jump);

	PlayerInputComponent->BindAction("IA_Slide", IE_Pressed, this, &ARunnerCharacter::Slide);

	PlayerInputComponent->BindAction("IA_Left", IE_Pressed, this, &ARunnerCharacter::MoveLeft);
	PlayerInputComponent->BindAction("IA_Right", IE_Pressed, this, &ARunnerCharacter::MoveRight);
}

void ARunnerCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Other->GetName());

	if (!Other->ActorHasTag(FName("Floor")))
	{
		if (!HasDied)
		{
			Death();
		}
	}
}

// Run forwards forever
void ARunnerCharacter::InfiniteRun()
{
	AddMovementInput(GetActorForwardVector());
}

void ARunnerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	float NewSpeed = GetCharacterMovement()->MaxWalkSpeed * 1.01;

	GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(NewSpeed, 700.f, 1500.f);

	Score = Score++;
	Score++;

}

void ARunnerCharacter::Death()
{
	HasDied = true;

	GetCharacterMovement()->DisableMovement();
	DisableInput(PlayerController);
	GetMesh()->SetSimulatePhysics(true);
	PlayerController->SetShowMouseCursor(true);
}



// =====================
// Slide functions
// =====================

// Master slide function
void ARunnerCharacter::Slide()
{
	GetCharacterMovement()->AddImpulse(FVector(0.f, 0.f, -1000.f), true);
	
	if(CanSlide())
	{
		BeginSlide();
	}
}

// Timer for slide
void ARunnerCharacter::CalculateSlideTimer(float dT)
{
	if (IsSliding)
	{
		SlideTimer += dT;

		if (SlideTimer >= 0.7f)
		{
			EndSlide();
		}
	}
}

// Checks if player can slide (isn't falling)
bool ARunnerCharacter::CanSlide()
{
	if (GetCharacterMovement()->Velocity.Size() > 60.f && !GetCharacterMovement()->IsFalling())
	{
		return true;
	}

	return false;
}

// Begins slide function disabling input
void ARunnerCharacter::BeginSlide()
{
	IsSliding = true;

	GetCapsuleComponent()->SetCollisionProfileName(FName("PawnSliding"));

	DisableInput(PlayerController);
}

// Resumes to preslide state
void ARunnerCharacter::EndSlide()
{
	IsSliding = false;
	SlideTimer = 0.f;

	GetCapsuleComponent()->SetCollisionProfileName(FName("Pawn"));

	EnableInput(PlayerController);
}

// =====================
// Movement functions
// =====================
void ARunnerCharacter::TimelineUpdate(float Value)
{
	FVector Location = GetActorLocation();

	Location.Y = FMath::Lerp(CurrentYPosition, LanePositions[TargetLaneIndex], Value);

	SetActorLocation(Location);
}

void ARunnerCharacter::TimelineFinished()
{
	CurrentLaneIndex = TargetLaneIndex;
}

void ARunnerCharacter::StartTimeline()
{
	if (MoveTimeline && MoveFloatCurve)
	{
		MoveTimeline->PlayFromStart();
	}
}

void ARunnerCharacter::ChangeLanes(int Direction)
{
	TargetLaneIndex = FMath::Clamp( (CurrentLaneIndex + Direction), 0, 2);

	CurrentYPosition = GetActorLocation().Y;

	StartTimeline();
}

