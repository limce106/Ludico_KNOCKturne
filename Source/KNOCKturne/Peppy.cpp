// Fill out your copyright notice in the Description page of Project Settings.
#include "Peppy.h"
#include "PeppyStatComponent.h"

// Sets default values
APeppy::APeppy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	SpringArm->TargetArmLength = 800.0f;
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bDoCollisionTest = false;

	InteractionCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractionCollider"));
	InteractionCollider->SetupAttachment(RootComponent);
	InteractionCollider->SetCapsuleRadius(96.0f);
	InteractionCollider->SetCapsuleHalfHeight(48.0f);

	
	PeppyController = UGameplayStatics::GetPlayerController(this, 0);

	PeppyStat = CreateDefaultSubobject<UPeppyStatComponent>(TEXT("PeppyStat"));

	// Initialize variables
	IsMove = false;
	IsSlide = false;

	CanSlide = true;

	SlideCooldown = 3.0f;
}

// Called when the game starts or when spawned
void APeppy::BeginPlay()
{
	Super::BeginPlay();

//	InteractionCollider->OnComponentBeginOverlap.AddDynamic(this, &APeppy::OnOverlapBegin);
//	InteractionCollider->OnComponentEndOverlap.AddDynamic(this, &APeppy::OnOverlapEnd);
	
}

// Called every frame
void APeppy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SlideHandling(DeltaTime);
}

void APeppy::PostInitializeComponents() {
	Super::PostInitializeComponents();

	PeppyStat->OnHPIsZero.AddLambda([this]() ->void {
		NTLOG(Warning, TEXT("OnHPIsZero"));
		SetActorEnableCollision(false);
		});
}

void APeppy::MoveForward(float Value) {
	if ((Controller != nullptr) && (Value != 0.0f)) {
		/* Move input mouse with keyboard
		if (isMove) {
			GetMovementComponent()->StopMovementImmediately();
			isMove = false;
		}
		*/
		if (!IsSlide) {
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}
}

void APeppy::MoveRight(float Value) {
	if ((Controller != nullptr) && (Value != 0.0f)) {
		/* Move input mouse with keyboard
		if (isMove) {
			GetMovementComponent()->StopMovementImmediately();
			isMove = false;
		}
		*/
		if (!IsSlide) {
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);
		}
	}
}

/*
* Move input mouse with keyboard
void APeppy::SetDestination() {
	isMove = true;
}
*/

void APeppy::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor && (OtherActor != this) && OtherComp) {
		if (OtherActor->ActorHasTag("NPC")) {
			UE_LOG(LogTemp, Warning, TEXT("NPC overlapped"));
		}
	}
}

void APeppy::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor && (OtherActor != this) && OtherComp) {
		if (OtherActor->ActorHasTag("NPC")) {
			UE_LOG(LogTemp, Warning, TEXT("NPC out of range"));
		}
	}
}

void APeppy::CheckInteraction() {
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(100.f),
		Params
	);

	if (bResult) {
		if (HitResult.GetActor()) {
		}
	}
}

// Called to bind functionality to input
void APeppy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &APeppy::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &APeppy::MoveRight);
	/* Move input mouse with keyboard
	PlayerInputComponent->BindAction("SetDestination", IE_Pressed, this, &APeppy::SetDestination);
	*/

	InputComponent->BindAction("Sliding", EInputEvent::IE_Pressed, this, &APeppy::SlideAction);
}


void APeppy::SlideAction() {
	if (CanSlide) {
		if (FollowTime <= ShortPressThreshold) {
			IsSlide = true;
			CanSlide = false;

			LeftSlideCooltime = SlideCooldown;

			FVector HitLocation = FVector::ZeroVector;
			FHitResult Hit;
			PeppyController->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
			HitLocation = Hit.Location;

			FRotator RotateDegree = FRotator(0.0f, (HitLocation - GetActorLocation()).Rotation().Yaw, 0.0f);
			GetCapsuleComponent()->SetWorldRotation(RotateDegree);
			//		UE_LOG(LogTemp, Warning, TEXT("%lf %lf %lf, %lf %lf %lf"), HitLocation.X, HitLocation.Y, HitLocation.Z, GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);

			FVector Direction = FRotationMatrix(FRotator(0, RotateDegree.Yaw, 0)).GetUnitAxis(EAxis::X);
			LaunchCharacter(Direction * 3000, false, true);

		}
	}
}

void APeppy::SlideHandling(float DeltaTime) {
	if (LeftSlideCooltime > 0.0f) {
		FVector velocity = GetMovementComponent()->Velocity;
		float CurMoveVelocity = sqrt((velocity.X * velocity.X) + (velocity.Y * velocity.Y));
		if (CurMoveVelocity > 600.0f) {
			IsSlide = true;
		}
		else {
			IsSlide = false;
		}
		LeftSlideCooltime -= DeltaTime;
	}
	else {
		LeftSlideCooltime = 0.0f;
		CanSlide = true;
	}
}
