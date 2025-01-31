// Fill out your copyright notice in the Description page of Project Settings.


#include "PeppySkillComponent.h"

UPeppySkillComponent::UPeppySkillComponent()
{
	SkillTableIndex = 0;
}


void UPeppySkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPeppySkillComponent::EnterBattleLevel() {
	LoadGameInstances();
	LoadCanUsePeppySkillList();
}

void UPeppySkillComponent::LoadGameInstances() {
	GameInstance = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	BattleTableManager = GameInstance->GetSubsystem<UBattleTableManagerSystem>();
}

void UPeppySkillComponent::LoadCanUsePeppySkillList() {
	FString CurrentLevel = GetWorld()->GetMapName();
	AddCanUsePeppySkillWithLevelName("Tutorial");
	if (CurrentLevel.Contains("LV_Tutorial")) {
		return;
	}
	AddCanUsePeppySkillWithLevelName("Episode1");
	if (CurrentLevel.Contains("LV_Battle")) {
		return;
	}
}

void UPeppySkillComponent::AddCanUsePeppySkillWithLevelName(FString LevelName) {
	UDataTable* PeppySkillTable = BattleTableManager->GetPeppySkillTable();

	TArray<FPeppySkillData*> PeppySkillRows;
	PeppySkillTable->GetAllRows<FPeppySkillData>("Get all rows of PeppySkillData", PeppySkillRows);
	if (PeppySkillRows.Num() == 0) {
		NTLOG(Error, TEXT("Failed to load PeppySkillTable"));
		return;
	}

	while (SkillTableIndex < PeppySkillRows.Num() && PeppySkillRows[SkillTableIndex]->SkillGroupCode == LevelName) {
		CanUsePeppySkillList.Add(*PeppySkillRows[SkillTableIndex]);
		SkillTableIndex++;
	}
}
