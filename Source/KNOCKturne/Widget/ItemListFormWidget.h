// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"

#include "ItemListFormWidget.generated.h"

/**
 * 
 */
UCLASS()
class KNOCKTURNE_API UItemListFormWidget : public UUserWidget
{
	GENERATED_BODY()
	void NativePreConstruct();
	void NativeConstruct();

protected:
	TArray<UUserWidget*> PackageSkillWidgetArr;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* icon_checkbox_selected;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* icon_checkbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Background;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_Background;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_CheckBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_IconBackground;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Count;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> PackageSkillWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPackageSkillWidget* PackageSkillWidget;

	UFUNCTION()
	void OnClicked_Skill();
};
