// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueManagerSystem.h"

UDialogueManagerSystem::UDialogueManagerSystem() {
	FString StringTablePath = TEXT("/Game/Assets/DataTable/StringTable.StringTable");
	FString DialogueStartIndexTablePath = TEXT("/Game/Assets/DataTable/StartIndexTable.StartIndexTable");
	FString SkillBuffStringTablePath = TEXT("/Game/Assets/DataTable/SkillBuffStringTable.SkillBuffStringTable");
	//Load StringTable
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_STRINGTABLE(*StringTablePath);
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_STARTINDEXTABLE(*DialogueStartIndexTablePath);
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SKILLBUFFSTRINGTABLE(*SkillBuffStringTablePath);
	NTCHECK(DT_STRINGTABLE.Succeeded());
	NTCHECK(DT_STARTINDEXTABLE.Succeeded());
	NTCHECK(DT_SKILLBUFFSTRINGTABLE.Succeeded());

	StringTable = DT_STRINGTABLE.Object;
	StartIndexTable = DT_STARTINDEXTABLE.Object;
	SkillBuffStringTable = DT_SKILLBUFFSTRINGTABLE.Object;

	DialogueMap.Add(TEXT("Dialogue_Prologue"), TEXT("/Game/Assets/DataTable/Dialogue_Prologue.Dialogue_Prologue"));
	DialogueMap.Add(TEXT("Dialogue_Npc"), TEXT("/Game/Assets/DataTable/Dialogue_Npc.Dialogue_Npc"));
	DialogueMap.Add(TEXT("Dialogue_EP1_Battle"), TEXT("/Game/Assets/DataTable/Dialogue_EP1_Battle.Dialogue_EP1_Battle"));
}

void UDialogueManagerSystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
}

UDataTable* UDialogueManagerSystem::LoadDialogueTable(FString TableName, int& Index) {
	FString DialoguePath = DialogueMap[TableName];
//	NTLOG(Warning, TEXT("Loaded Table : %s"), *DialoguePath);
	Index = (StartIndexTable->FindRow<FStartIndex>(*TableName, TEXT(""))->StringIndex) - 1;

	return LoadObject<UDataTable>(NULL, *DialoguePath, NULL, LOAD_None, NULL);
}

UDataTable* UDialogueManagerSystem::GetStringTable() {
	return StringTable;
}

UDataTable* UDialogueManagerSystem::GetStartIndexTable() {
	return StartIndexTable;
}

FString UDialogueManagerSystem::GetString2(int index) {
	TArray<FDialogueString*> StringRows;
	StringTable->GetAllRows<FDialogueString>("GetAllRows", StringRows);
	return StringRows[index]->KOR;
}