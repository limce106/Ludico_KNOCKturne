// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KNOCKturne.h"
#include "GameFramework/Actor.h"

#include "GameInstance/BattleTableManagerSystem.h"
#include "GameInstance/BattleManagerSystem.h"
#include "BattleManager.h"

#include "CommonSkillActor.generated.h"

UENUM(BlueprintType)
enum class ESkillUsingType : uint8 {
	Sequential		UMETA(DisplayName = "Sequential"),
	Probabilistic	UMETA(DisplayName = "Probabilistic")
};

UCLASS()
class KNOCKTURNE_API ACommonSkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACommonSkillActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flag")
	bool IsHitPlayer = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flag")
	bool IsInitialized = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Type")
	ESkillUsingType CurSkillUsingType = ESkillUsingType::Sequential;

	class UBattleTableManagerSystem* BattleTableManagerSystem;
	class UBattleManagerSystem* BattleManagerSystem;
	class ABattleManager* BattleManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* SkillHitArea;
	
	UFUNCTION(BlueprintCallable)
	FName GetCurrentBlueprintClassName();

	virtual void Initialize();

	virtual bool TryOperateSkillEffect(ESkillUsingType SkillUsingType);

private:
	void RegistSkillActorOnBattleManager();

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CustomSkillOperation(int32 EffectSequence, AActor* TargetActor, FCurEffectIndexSkillData SkillData, AActor* SkillActor);
};
