// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleManager.h"
#include "Peppy.h"
#include "Boss.h"
#include "Component/PeppyStatComponent.h"
#include "Component/BossStatComponent.h"
#include "GameInstance/BattleTableManagerSystem.h"
#include "GameInstance/BattleManagerSystem.h"
#include "GameInstance/ActorManagerSystem.h"
#include "GameMode/NTBattleGameMode.h"
#include "LevelScriptActor/BattleFieldLevelScriptActor.h"

// Sets default values
ABattleManager::ABattleManager()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	CurrentTurnType = BossTurn;
	LeftCurrentTurnTime = 0;

	static ConstructorHelpers::FClassFinder<ABoss> BP_BossActorClass(TEXT("/Game/Blueprints/Actors/Battle/NPC/BP_BossRose"));
	BossActorSubClass = BP_BossActorClass.Class;

//	static ConstructorHelpers::FClassFinder<AActor> BP_PS_AmbiguousEmotion(TEXT("/Game/Blueprints/Skills/Peppy/BP_PS_AmbiguousEmotion"));
//	auto AmbiguousEmotion_Ref = BP_PS_AmbiguousEmotion.Class;
}

void ABattleManager::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunTurnTimer(DeltaTime);
}

void ABattleManager::StartBossTurn() {
	isPeppyTurn = false;
	ProcessDamageBeforeStartTurn();
	SetLeftCurrentTurnTime(ActorManagerSystem->BossActor->StatComponent->CurStatData.Turn);
	ActorManagerSystem->BossActor->SpawnBossSkill();
	BP_StartBossTurn();
	SetBossAndResponseStance();
	ActorManagerSystem->BossActor->SetActorLocation(FVector(1600.0f, 760.0f, -850.0f));
}

/* �ش� �޼���� ���� �������Ʈ ��� �ʱ�ȭ ȣ�� Ÿ�̹� ���� ���� ���� ���� 1ȸ�� ���� �����մϴ�.������ Turn ���� ������ ���� C++ Ŭ���� �������� �̷�����ϴ�. */
void ABattleManager::BP_InitStartBattle() {
	if (IsCalled_InitStartBossTurn) {
		NTLOG(Error, TEXT("StartBossTurn method has already been called. This method can only be called once on blueprint."));
		return;
	}
	SetActorTickEnabled(true);
	IsCalled_InitStartBossTurn = true;
	ActorManagerSystem->PeppyActor->BuffComponent->UseStarlightNum = 0;
	StartBossTurn();
	BattleManagerSystem->StartBattleTime = FDateTime::Now();
}

void ABattleManager::StartPeppyTurn() {
	isPeppyTurn = true;
	SetLeftCurrentTurnTime(ActorManagerSystem->PeppyActor->StatComponent->CurStatData.Turn);
	BP_StartPeppyTurn();
	ActorManagerSystem->BossActor->SetActorLocation(FVector(1600.0f, 760.0f, -750.0f));
}


void ABattleManager::TurnChange() {
	for (auto SkillActor : SkillActorsOnField) {
		if (SkillActor.Value) {
			SkillActor.Value->Destroy();
		}
	}
	SkillActorsOnField.Empty();

	switch (CurrentTurnType) {
	case BossTurn:
		ActorManagerSystem->BossActor->ClearUseSkillTimer();
		StartPeppyTurn();
		CurrentTurnType = PeppySkillSelectingTurn;
		ActorManagerSystem->BossActor->EnergySpawner->DeactivateAllSpawnedObject();

		break;
	case PeppySkillSelectingTurn:
		ActorManagerSystem->BossActor->PauseAndClearUseSkillTimer();
		ActorManagerSystem->PeppyActor->BuffComponent->CanGetMoodBuff = false;
		SetLeftCurrentTurnTime(100);	// ��ų ����� ���� �ӽ� �� Ÿ�� ����
		BP_UsePeppySkills();
		CurrentTurnType = PeppySkillUsingTurn;
		break;
	case PeppySkillUsingTurn:
		if (ActorManagerSystem->BossActor->IsDie) {
			Cast<ANTBattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GameClear();
			break;
		}
		ActorManagerSystem->BossActor->SpawnBossSkillWhenStartBossTurn();
		StartBossTurn();
		CurrentTurnType = BossTurn;
		BattleManagerSystem->UpdateRoundInfo();

		ActorManagerSystem->BossActor->BuffComponent->ElapseTurn();
		ActorManagerSystem->PeppyActor->BuffComponent->ElapseTurn();
		ActorManagerSystem->SpecialSkillActor->ElapseTurn();
		
		break;
	}
}

void ABattleManager::RunTurnTimer(float DeltaTime) {
	if (LeftCurrentTurnTime <= 0.0f) {
		TurnChange();
	}
	LeftCurrentTurnTime -= DeltaTime;

	/*
	Timer example

	LeftTurnTimeTimerManager.SetTimer(
		LeftTurnTimeTimer,
		this,
		&UBattleSystemComponent::DecreaseLeftCurrentTurnTime,
		1.0f,
		true,
		(float)CurrentTurnTime
	);
	*/
}
void ABattleManager::DecreaseLeftCurrentTurnTime() {
	if (LeftCurrentTurnTime >= 0) {
		LeftCurrentTurnTime--;
	}
}

void ABattleManager::ProcessDamageBeforeStartTurn() {
	if (ActorManagerSystem->PeppyActor->DamageArrayEachTurn.IsEmpty()) {
		return;
	}

	TMap<FString, int32> CurrentTurnDamages = ActorManagerSystem->PeppyActor->DamageArrayEachTurn[0];
	ActorManagerSystem->PeppyActor->DamageArrayEachTurn.RemoveAt(0);

	int32 TotalDamage = 0;
	for (auto DamageData : CurrentTurnDamages) {
		TotalDamage += DamageData.Value;
	}

	NTLOG(Warning, TEXT("Peppy get damaged starting boss turn %d"), TotalDamage);
	ActorManagerSystem->PeppyActor->StatComponent->TryUpdateCurStatData(FStatType::EP, -TotalDamage);
}

float ABattleManager::GetLeftCurrentTurnTime() {
	return LeftCurrentTurnTime;
}

void ABattleManager::SetLeftCurrentTurnTime(float TurnTime) {
	LeftCurrentTurnTime = TurnTime;
}

void ABattleManager::EndTurn() {
	LeftCurrentTurnTime = 0;
}

void ABattleManager::EndBattle() {
	SetActorTickEnabled(false);
	TurnChange();
}

void ABattleManager::SetBossAndResponseStance()
{
	BattleManagerSystem->BossStanceID = BattleManagerSystem->BossStances[FMath::Rand() % BattleManagerSystem->BossStances.Num()];
	BattleManagerSystem->ResponseStanceID = BattleTableManagerSystem->BossStanceTable->FindRow<FBossStanceData>(*("Stance_" + BattleManagerSystem->BossStanceID), TEXT(""))->ResponseStanceID;
	NTLOG(Warning, TEXT("This Round Boss Stance: %s"), *BattleManagerSystem->BossStanceID);
}

void ABattleManager::Init() {
	UGameInstance* GameInstance = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	BattleTableManagerSystem = GameInstance->GetSubsystem<UBattleTableManagerSystem>();
	BattleTableManagerSystem->BattleManager = this;
	BattleManagerSystem = GameInstance->GetSubsystem<UBattleManagerSystem>();
	BattleManagerSystem->BattleManager = this;
	ActorManagerSystem = GameInstance->GetSubsystem<UActorManagerSystem>();

	ActorManagerSystem->BossActor = Cast<ABoss>(UGameplayStatics::GetActorOfClass(GetWorld(), BossActorSubClass));
	ActorManagerSystem->PeppyActor = Cast<APeppy>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ActorManagerSystem->BattleManager = this;
	ActorManagerSystem->SpecialSkillActor = GetWorld()->SpawnActor<ASpecialSkillActor>(SpecialSkillClass, FVector(1600.0f, 760.0f, -850.0f), FRotator(0.0f, 0.0f, 0.0f));
	ActorManagerSystem->ItemActor = GetWorld()->SpawnActor<AItemActor>(ItemClass, FVector(1600.0f, 760.0f, -850.0f), FRotator(0.0f, 0.0f, 0.0f));
	BP_ActorInit();

	HandledBuffComponents.Add(ActorManagerSystem->BossActor->BuffComponent);
	HandledBuffComponents.Add(ActorManagerSystem->PeppyActor->BuffComponent);
	Cast<ABattleFieldLevelScriptActor>(GetWorld()->GetLevelScriptActor())->LevelPlay();
}
