// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueManagerSystem.h"

UDialogueManagerSystem::UDialogueManagerSystem() {
	FString StringTablePath = TEXT("/Game/Assets/DataTable/StringTable.StringTable");
	FString DialogueStartIndexTablePath = TEXT("/Game/Assets/DataTable/StartIndexTable.StartIndexTable");
	//Load StringTable
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_STRINGTABLE(*StringTablePath);
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_STARTINDEXTABLE(*DialogueStartIndexTablePath);
	NTCHECK(DT_STRINGTABLE.Succeeded());
	NTCHECK(DT_STARTINDEXTABLE.Succeeded());

	StringTable = DT_STRINGTABLE.Object;
	StartIndexTable = DT_STARTINDEXTABLE.Object;
	DialogueMap.Add(TEXT("Dialogue_Prologue"), TEXT("/Game/Assets/DataTable/Dialogue_Prologue.Dialogue_Prologue"));
	DialogueMap.Add(TEXT("Dialogue_Npc"), TEXT("/Game/Assets/DataTable/Dialogue_Npc.Dialogue_Npc"));
}

void UDialogueManagerSystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);

	NTLOG(Warning, TEXT("Test"));
}

UDataTable* UDialogueManagerSystem::LoadDialogueTable(FString TableName, int& Index) {
	NTLOG(Warning, TEXT("%s"), *TableName);
	FString DialoguePath = DialogueMap[TableName];
	NTLOG(Warning, TEXT("%s"), *DialoguePath);
	Index = (StartIndexTable->FindRow<FStartIndex>(*TableName, TEXT(""))->StringIndex) - 1;

	return LoadObject<UDataTable>(NULL, *DialoguePath, NULL, LOAD_None, NULL);
//	return LoadClass<UDataTable>(NULL, *DialoguePath)->GetDefaultObject<UDataTable>();
}

UDataTable* UDialogueManagerSystem::GetStringTable() {
	return StringTable;
}