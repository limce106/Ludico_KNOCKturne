// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KNOCKturne.h"
#include "Engine/DataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BattleManagerSystem.generated.h"

/**
 * 
 */


UCLASS()
class KNOCKTURNE_API UBattleManagerSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UBattleManagerSystem();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flag")
	bool VisibleBossSkillHitArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	class UStaticMeshComponent* CurBossSkillHitArea;

	UClass* FootholdClass;
	TSubclassOf<AActor> FootholdSubclass;
	AActor* FootholdObject;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 LeftCurTurnTime;

	void InitializeFootholdObject();

	UFUNCTION(BlueprintCallable)
	void SpawnFoothold();

	UFUNCTION(BlueprintCallable)
	void SetTimerUnvisibleHitArea();
};