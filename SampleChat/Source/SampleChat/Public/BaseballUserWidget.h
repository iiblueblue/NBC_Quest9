// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseballUserWidget.generated.h"

class UEditableTextBox;
class UTextBlock;
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

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
};
