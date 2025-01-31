// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KNOCKturne.h"
#include "Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"

UENUM(BlueprintType)
enum class EBuffType : uint8 {
	DefenseIncrease		UMETA(DisplayName = "DefenseIncrease"),
	AttackIncrease		UMETA(DisplayName = "AttackIncrease"),
	AvdIncrease			UMETA(DisplayName = "AvdIncrease"),
	ReflexiveAttack		UMETA(DisplayName = "ReflexiveAttack"),
	ReflexiveRecovery	UMETA(DisplayName = "ReflexiveRecovery"),
	PeriodicRecovery	UMETA(DisplayName = "PeriodicRecovery"),
	AttackDecrease		UMETA(DisplayName = "AttackDecrease"),
	EnergyDropIncrease	UMETA(DisplayName = "EnergyDropIncrease"),
	PeriodicAttack		UMETA(DisplayName = "PeriodicAttack"),
	Blind				UMETA(DisplayName = "Blind"),
	SpeedDecrease		UMETA(DisplayName = "SpeedDecrease"),
	Confuse				UMETA(DisplayName = "Confuse"),
	Seal				UMETA(DisplayName = "Seal"),
	IntervalIncrease	UMETA(DisplayName = "IntervalIncrease"),
	SpecialMpIncrease	UMETA(DisplayName = "SpecialMpIncrease"),
	Shield				UMETA(DisplayName = "Shield"),
	Revive				UMETA(DisplayName = "Revive"),
	Mood				UMETA(DisplayName = "Mood"),
	Warning				UMETA(DisplayName = "Warning"),
	RecoveryEnergy		UMETA(DisplayName = "RecoveryEnergy"),
	SpeedIncrease		UMETA(DisplayName = "SpeedIncrease"),
	DefenseDecrease		UMETA(DisplayName = "DefenseDecrease"),
	Stun				UMETA(DisplayName = "Stun"),
	RestrictRecovery	UMETA(DisplayName = "RestrictRecovery"),
	Friction			UMETA(DisplayName = "Friction")
};

UENUM(BlueprintType)
enum class EBuffTermType : uint8 {
	Turn	UMETA(DisplayName = "Turn"),
	Second	UMETA(DisplayName = "Second")
};

UENUM(BlueprintType)
enum class EBuffPositivenessType : uint8 {
	Positive	UMETA(DisplayName = "Positive"),
	Negative	UMETA(DisplayName = "Negative")
};

USTRUCT(BlueprintType)
struct FBuffData {
	GENERATED_BODY()
	
public:
	FBuffData() : SourceId("-1"), BuffType(0), BuffTermType(EBuffTermType::Turn), Duration(0), Value_N(0), Value_M(0), Value_T(0) {}

	FBuffData(FString SourceId, int32 BuffType, EBuffTermType BuffDurationType, int32 Duration, float Value_N, float Value_M, float Value_T) {
		this->SourceId = SourceId;
		this->BuffType = BuffType;
		this->BuffTermType = BuffDurationType;
		this->Duration = Value_T;
		this->Value_N = Value_N;
		this->Value_M = Value_M;
		this->Value_T = Value_T;
	}
	
	FBuffData(FString SourceId, FBuffTable* BuffTableData) {
		this->SourceId = SourceId;

		bool isSecondType = false;
		for (auto Buff : BuffNamePerSecondStr) {
			if (Buff == BuffTableData->BuffName) {
				isSecondType = true;
				break;
			}
		}

		this->BuffType = BuffTableData->BuffType;
		this->BuffTermType = isSecondType ? EBuffTermType::Second : EBuffTermType::Turn;
		this->Duration = BuffTableData->defaultT;
		this->Value_N = BuffTableData->defaultN;
		this->Value_M = BuffTableData->defaultM;
		this->Value_T = BuffTableData->defaultT;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString SourceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 BuffType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EBuffTermType BuffTermType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Duration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Value_N;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Value_M;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Value_T;

	FBuffData* CreateBuffData(FString _SourceId, FBuffTable* BuffTableData) {
		// TArray<EBuffType>의 Contains 메서드로 간단하게 구현할 수 있을 줄 알았는데, uenum이 == operator를 지원하지 않음.
		// 해당 연산자 오버로딩을 하려 했지만, enum class 안에 메서드 구현도 불가능함. 
		// 그래서 어쩔 수 없이 아래와 같이 배열 전체를 순회하며 uint32 타입으로의 강제 캐스팅을 통해 비교함.
		bool isSecondType = false;
		for (auto Buff : BuffNamePerSecondStr) {
			if (Buff == BuffTableData->BuffName) {
				isSecondType = true;
				break;
			}
		}

		return new FBuffData(_SourceId, BuffTableData->BuffType, isSecondType ? EBuffTermType::Second : EBuffTermType::Turn,
			BuffTableData->defaultT, BuffTableData->defaultN, BuffTableData->defaultM, BuffTableData->defaultT);
	}

	TArray<EBuffType> BuffListPerTurn = {
		EBuffType::DefenseIncrease,
		EBuffType::AttackIncrease,
		EBuffType::AvdIncrease,
		EBuffType::ReflexiveAttack,
		EBuffType::ReflexiveRecovery,
		EBuffType::EnergyDropIncrease,
		EBuffType::AttackDecrease,
		EBuffType::Seal,
		EBuffType::IntervalIncrease,
		EBuffType::SpecialMpIncrease,
		EBuffType::Shield,
		EBuffType::Revive,
		EBuffType::Mood,
		EBuffType::Warning,
		EBuffType::RecoveryEnergy,
		EBuffType::PeriodicRecovery,
		EBuffType::SpeedIncrease,
		EBuffType::DefenseDecrease,
		EBuffType::RestrictRecovery,
		EBuffType::Friction
	};

	TArray<EBuffType> BuffListPerSecond = {
		EBuffType::PeriodicAttack,
		EBuffType::Blind,
		EBuffType::Confuse,
		EBuffType::SpeedDecrease,
		EBuffType::Stun
	};

	TArray<FString> BuffNamePerSecondStr = {
		"BF_PeriodicAttack",
		"BF_Blind",
		"BF_Confuse",
		"BF_SpeedDecrease",
		"BF_Stun"
	};
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KNOCKTURNE_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()
	class APeppy* Peppy;

public:
	UBuffComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<EBuffType, AActor*> TargetOfBuff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanGetMoodBuff = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 AdditionalEnergyByBuff = 0;
	/*'아름다운 별빛' 사용 횟수*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UseStarlightNum = 0;
	/*'지속 회복' 회복 중*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isRecoveringEP = false;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Has")
	TMap<EBuffType, FBuffData> HasPositiveBuffs_PerTurn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Has")
	TMap<EBuffType, FBuffData> HasPositiveBuffs_PerSecond;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Has")
	TMap<EBuffType, FBuffData> HasNegativeBuffs_PerTurn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Has")
	TMap<EBuffType, FBuffData> HasNegativeBuffs_PerSecond;

	class UNiagaraSystem* NS_PretendNotSick_2;
	class UNiagaraSystem* NS_ShieldBreak;
	class UNiagaraSystem* NS_GatheringEnergy;
	class UNiagaraSystem* NS_GatheringEnergy_2;

private:
	const TMap<EBuffType, FString> BuffTypeToStringMap = {
		{ EBuffType::DefenseIncrease,	"DefenseIncrease" },
		{ EBuffType::AttackIncrease,	"AttackIncrease" },
		{ EBuffType::AvdIncrease,		"AvdIncrease" },
		{ EBuffType::ReflexiveAttack,	"ReflexiveAttack"},
		{ EBuffType::ReflexiveRecovery,	"ReflexiveRecovery"},
		{ EBuffType::PeriodicRecovery,	"PeriodicRecovery"},
		{ EBuffType::EnergyDropIncrease,"EnergyDropIncrease"},
		{ EBuffType::AttackDecrease,	"AttackDecrease" },
		{ EBuffType::PeriodicAttack,	"PeriodicAttack"},
		{ EBuffType::Blind,				"Blind" },
		{ EBuffType::SpeedDecrease,		"SpeedDecrease" },
		{ EBuffType::Confuse,			"Confuse" },
		{ EBuffType::Seal,				"Seal" },
		{ EBuffType::IntervalIncrease,	"IntervalIncrease" },
		{ EBuffType::SpecialMpIncrease,	"SpecialMpIncrease" },
		{ EBuffType::Shield,			"Shield" },
		{ EBuffType::Revive,			"Revive" },
		{ EBuffType::Mood,				"Mood" },
		{ EBuffType::Warning,			"Warning" },
		{ EBuffType::SpeedIncrease,		"SpeedIncrease" },
		{ EBuffType::DefenseDecrease,	"DefenseDecrease" },
		{ EBuffType::Stun,				"Stun" },
		{ EBuffType::RestrictRecovery,	"RestrictRecovery" },
		{ EBuffType::RecoveryEnergy,	"RecoveryEnergy" },
		{ EBuffType::Friction,			"Friction" }
	};

	const TMap<FString, int32> BuffIconNameToRowNumMap = {
		{ "Icon_buff_DefenseIncrease", 0 },
		{ "Icon_buff_AttackIncrease", 1 },
		{ "Icon_buff_AvdIncrease", 2 },
		{ "Icon_buff_ReflexiveAttack", 3 },
		{ "Icon_buff_ReflexiveRecovery", 4 },
		{ "Icon_buff_PeriodicRecovery", 5 },
		{ "Icon_buff_EnergyDropIncrease", 6 },
		{ "Icon_buff_SpeedIncrease", 7 },
		{ "Icon_buff_DefenseDecrease", 8 },
		{ "Icon_buff_AttackDecrease", 9 },
		{ "Icon_buff_Stun", 10 },
		{ "Icon_buff_PeriodicAttack", 11 },
		{ "Icon_buff_RestrictRecovery", 12 },
		{ "Icon_buff_Blind", 13 },
		{ "Icon_buff_SpeedDecrease", 14 },
		{ "Icon_buff_Confuse", 15 },
		{ "Icon_buff_Seal", 16 },
		{ "Icon_buff_Friction", 17 },
		{ "Icon_buff_IntervalIncrease", 18 },
		{ "Icon_buff_SpecialMpIncrease", 19 },
		{ "Icon_buff_Shield", 20 },
		{ "Icon_buff_Revive", 21 },
		{ "Icon_buff_Mood", 22 },
		{ "Icon_buff_Warning", 23 },
		{ "Icon_buff_RecoveryEnergy", 24 },
	};

	class UBattleTableManagerSystem* BattleTableManagerSystem;
	class UActorManagerSystem* ActorManagerSystem;
	class ABattleManager* BattleManager;

	TMap<EBuffType, float> BuffTempDelayTime;
	/*휴식 회복 버프용: 2초 전 페피 위치*/
	FVector PrePeppyLocation;
	float TempDelayTime;
	FTimerHandle EffectHandle;
	bool isPeppyMove = false;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FString, EBuffType> BuffIconNameToBuffTypeMap = {
		{ "Icon_buff_DefenseIncrease", EBuffType::DefenseIncrease },
		{ "Icon_buff_AttackIncrease", EBuffType::AttackIncrease },
		{ "Icon_buff_AvdIncrease", EBuffType::AvdIncrease },
		{ "Icon_buff_ReflexiveAttack", EBuffType::ReflexiveAttack },
		{ "Icon_buff_ReflexiveRecovery", EBuffType::ReflexiveRecovery },
		{ "Icon_buff_PeriodicRecovery", EBuffType::PeriodicRecovery },
		{ "Icon_buff_EnergyDropIncrease", EBuffType::EnergyDropIncrease },
		{ "Icon_buff_SpeedIncrease", EBuffType::SpeedIncrease },
		{ "Icon_buff_DefenseDecrease", EBuffType::DefenseDecrease },
		{ "Icon_buff_AttackDecrease", EBuffType::AttackDecrease },
		{ "Icon_buff_Stun", EBuffType::Stun },
		{ "Icon_buff_PeriodicAttack", EBuffType::PeriodicAttack },
		{ "Icon_buff_RestrictRecovery", EBuffType::RestrictRecovery },
		{ "Icon_buff_Blind", EBuffType::Blind },
		{ "Icon_buff_SpeedDecrease", EBuffType::SpeedDecrease },
		{ "Icon_buff_Confuse", EBuffType::Confuse },
		{ "Icon_buff_Seal", EBuffType::Seal },
		{ "Icon_buff_Friction", EBuffType::Friction },
		{ "Icon_buff_IntervalIncrease", EBuffType::IntervalIncrease },
		{ "Icon_buff_SpecialMpIncrease", EBuffType::SpecialMpIncrease },
		{ "Icon_buff_Shield", EBuffType::Shield },
		{ "Icon_buff_Revive", EBuffType::Revive },
		{ "Icon_buff_Mood", EBuffType::Mood },
		{ "Icon_buff_Warning", EBuffType::Warning },
		{ "Icon_buff_RecoveryEnergy", EBuffType::RecoveryEnergy },
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<EBuffType, int32> OriginalDuration;

	UFUNCTION(BlueprintCallable)
	bool IsPositiveBuff(EBuffType BuffType);
	/* 자신에게 적용된 BuffTyp의 버프를 제거합니다.*/
	UFUNCTION(BlueprintCallable)
	bool RemoveBuff(EBuffType BuffType);
	/* 자신에게 적용된 랜덤한 긍정적 버프를 Num개 제거합니다.*/
	UFUNCTION(BlueprintCallable)
	void RemoveRandomPositiveBuff(int32 Num);
	/* 자신에게 적용된 랜덤한 부정적 버프를 Num개 제거합니다.*/
	UFUNCTION(BlueprintCallable)
	void RemoveRandomNegativeBuff(int32 Num);
	/* 자신에게 적용된 모든 긍정적 버프를 제거합니다.*/
	UFUNCTION(BlueprintCallable)
	void RemoveAllPositiveBuff();
	/* 자신에게 적용된 모든 부정적 버프를 제거합니다.*/
	UFUNCTION(BlueprintCallable)
	void RemoveAllNegativeBuff();
	/* 자신에게 적용된 모든 버프를 제거합니다.*/
	UFUNCTION(BlueprintCallable)	
	void RemoveAllBuff();
	/* 자신에게 TermType을 주기로 Duration만큼의 지속하는 BuffType의 버프를 부여합니다.*/
	UFUNCTION(BlueprintCallable) 
	void AcquireBuff(EBuffType BuffType, FCurEffectIndexSkillData SkillData);

	void ElapseTurn();
	void ElapseDeltaTime(float DeltaTime);
	
	void ExpireBuff(TMap<EBuffType, FBuffData>* BuffMap, EBuffType BuffType);

	UFUNCTION(BlueprintCallable)
	bool HasBuff(EBuffType BuffType);

	UFUNCTION(BlueprintCallable)
	int32 GetNegativeBuffNum();

	UFUNCTION(BlueprintCallable)
	void OperatePositiveBuffs_PerTurn(EBuffType BuffType);
	UFUNCTION(BlueprintCallable)
	void EndPositiveBuffs_PerTurn(EBuffType BuffType);

	UFUNCTION(BlueprintCallable)
	void OperatePositiveBuffs_PerSecond(EBuffType BuffType, float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void EndPositiveBuffs_PerSecond(EBuffType BuffType);

	UFUNCTION(BlueprintCallable)
	void OperateNegativeBuffs_PerTurn(EBuffType BuffType);
	UFUNCTION(BlueprintCallable)
	void EndNegativeBuffs_PerTurn(EBuffType BuffType);

	UFUNCTION(BlueprintCallable)
	void OperateNegativeBuffs_PerSecond(EBuffType BuffType, float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void EndNegativeBuffs_PerSecond(EBuffType BuffType);

	/*휴식 회복 버프 작동*/
	/*T턴동안 페피가 M초동안 가만히 있을 때마다 페피의 HP가 N만큼 회복*/
	/*턴 단위 버프 중 유일하게 초를 기반으로 효과가 발동하는 버프이다. 따라서 예외적으로 작동 함수를 구현히였다.*/
	UFUNCTION(BlueprintCallable)
	void TryOperatePeriodicRecovery(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	void OperateBuffs_PerSecond(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	FString BuffTypeToString(EBuffType BuffType);
	UFUNCTION(BlueprintCallable)
	int32 BuffIconNameToRowNum(FString BuffIconName);
	UFUNCTION()
	bool BuffDelayWithDeltaTime(EBuffType BuffType, float DelayTime, float DeltaSeconds);
	UFUNCTION()
	bool DelayWithDeltaTime(float DelayTime, float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	FBuffData GetBuffData(EBuffType BuffType);

	UFUNCTION(BlueprintCallable)
	void TryUpdateBuffDataBySkillData(EBuffType BuffType, FBuffData BuffData, float ValueN, float ValueM, float ValueT);
	/*쉴드 개수 반환*/
	UFUNCTION(BlueprintCallable)
	int GetShieldNum();
	/*쉴드 개수 -1*/
	UFUNCTION(BlueprintCallable)
	void ReduceOneShield();
	/*혼란 버프 보유 여부 반환*/
	UFUNCTION(BlueprintCallable)
	bool HasConfuseBuff();

	/*버프 UI 갱신*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> HasBossBuff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> HasPeppyBuff;
	UFUNCTION(BlueprintCallable)
	void DeleteBuffUI(EBuffType BuffType);
	UFUNCTION(BlueprintCallable)
	int32 GetCurDuration(EBuffType BuffType);
	UFUNCTION(BlueprintCallable)
	bool isTermTypeTurn(EBuffType BuffType);
};
