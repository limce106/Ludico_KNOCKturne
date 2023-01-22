// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "Peppy.generated.h"

UCLASS()
class KNOCKTURNE_API APeppy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APeppy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction)
	UCapsuleComponent* InteractionCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	class UPeppyStatComponent* PeppyStat;
	
	// Time Threshold to know if it was a short press
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flag)
	bool CanMove;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float value);
	void MoveRight(float value);

	void SlideAction();
	void SlideHandling(float DeltaTime);

	void CheckInteraction();

private:
	APlayerController* PeppyController;

	bool IsSlide;

	bool CanSlide;

	float SlideCooldown;
	UPROPERTY(Transient, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float LeftSlideCooltime;

	float FollowTime;

	const float WEIGHTMOVESPEED = 0.5f;

	UPROPERTY(EditAnywhere)
	int32 SlidingSpeed;

	// 페피가 배틀 필드에서 끝에서 끝까지 이동하는 데 10초가 걸리는 속도
	const float StandardMaxWalkSpeed = 93.0f;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 5;
	float CurWalkSpeed;
};