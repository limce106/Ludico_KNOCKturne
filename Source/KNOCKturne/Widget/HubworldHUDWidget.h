// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Widget/DreamDiaryWidget.h"
#include "GameMode/KNOCKturneGameState.h"

#include "HubworldHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class KNOCKTURNE_API UHubworldHUDWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativePreConstruct();
	virtual void NativeConstruct();

	class UBattleManagerSystem* BattleManagerSystem;
	AKNOCKturneGameState* KNOCKturneGameState;

protected:
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Diary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Setting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Save;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_SettingText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_DiaryText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_SaveText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_DreamDiary_Updated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> DreamDiaryWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UDreamDiaryWidget* DreamDiaryWidgetRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> SaveAlertModalClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USaveAlertModalWidget* SaveAlertModalRef;

	UFUNCTION()
	void OnClick_ButtonDiary();
	UFUNCTION()
	void ClickSave();
	UFUNCTION()
	void ClickSaveAlertModalYes();
	UFUNCTION()
	void ClickSaveAlertModalNo();
};
