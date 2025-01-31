// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KNOCKturne.h"

#include "StatComponent.h"
#include "GameInstance/BattleTableManagerSystem.h"
#include "Components/ActorComponent.h"

#include "BossStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KNOCKTURNE_API UBossStatComponent : public UStatComponent
{
	GENERATED_BODY()

public:
	UBossStatComponent();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

private:
	class UBattleTableManagerSystem* BattleTableManagerSystem = nullptr;
	class UActorManagerSystem* ActorManagerSystem;

	class ABoss* BossActor;

	FBossStatData MinStatData;
	FBossStatData MaxStatData;

public:
	UPROPERTY(Transient, BlueprintReadOnly)
	FBossStatData CurStatData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool GetDamage = false;

	UFUNCTION(BlueprintCallable)
	void GetDamaged(float Value) override;

	UFUNCTION(BlueprintCallable)
	virtual bool TryUpdateCurStatData(FStatType StatType, float Value) override;

	void SetDefaultStat();

	UFUNCTION(BlueprintCallable)
	void CreateDamageText(float Value) override;

};
