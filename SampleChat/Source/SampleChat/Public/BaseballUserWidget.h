// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseballUserWidget.generated.h"

class UEditableTextBox;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class SAMPLECHAT_API UBaseballUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Timer;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Result;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Id;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Turn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Chance1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Chance2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Chance3;

	TArray<TObjectPtr<UImage>> Image_Chances;

protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetTimerText(const FText& NewText);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetResultText(const FText& NewText);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetIdText(const FText& NewText);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetTurnText(const FText& NewText);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetTurnImage(const int32 ThisTurn);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetTurnTextColor(bool bIsMyTurn);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetEditableBoxReadOnly(bool bIsMyTurn);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
};
