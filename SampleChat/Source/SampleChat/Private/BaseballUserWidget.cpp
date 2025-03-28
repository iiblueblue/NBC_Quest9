// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballUserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/Text/TextLayout.h"
#include "BaseballPlayerController.h"
#include "MyBlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UBaseballUserWidget::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController))
		{
			ABaseballPlayerController* OwningBaseballPlayerController = Cast<ABaseballPlayerController>(OwningPlayerController);
			if (IsValid(OwningBaseballPlayerController))
			{
				if (UMyBlueprintFunctionLibrary::bIsResponse(Text.ToString()))
				{
					OwningBaseballPlayerController->SetChatMessageString(Text.ToString());
				}
				else
				{
					UKismetSystemLibrary::PrintString(this, TEXT("Wrong Text"), true, true, FLinearColor::Red, 5.0f);
				}

				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}

void UBaseballUserWidget::SetTimerText(const FText& NewText)
{
	if (TextBlock_Timer)
	{
		TextBlock_Timer->SetText(NewText);
	}
}

void UBaseballUserWidget::SetResultText(const FText& NewText)
{
	if (TextBlock_Result)
	{
		TextBlock_Result->SetText(NewText);
	}
}

void UBaseballUserWidget::SetIdText(const FText& NewText)
{
	if (TextBlock_Id)
	{
		TextBlock_Id->SetText(NewText);
	}
}

void UBaseballUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UBaseballUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}
