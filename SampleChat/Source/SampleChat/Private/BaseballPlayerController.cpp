// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballPlayerController.h"
#include "BaseballUserWidget.h"
#include "MyBlueprintFunctionLibrary.h"
#include "BaseballGameMode.h"
#include "BaseballPlayerState.h"

void ABaseballPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	CreateUserWidget();
	ABaseballPlayerState* MyPlayerState = GetPlayerState<ABaseballPlayerState>();
	if (MyPlayerState)
	{
		DisplayUserId(MyPlayerState->UserId);
	}
}

void ABaseballPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;
	
	ServerRPCSendChatMessage(InChatMessageString);
}

void ABaseballPlayerController::ClientRPCReceiveChatMessage_Implementation(const FString& InChatMessageString)
{
	DisplayResult(InChatMessageString);
}

void ABaseballPlayerController::ServerRPCSendChatMessage_Implementation(const FString& InChatMessageString)
{
	ABaseballGameMode* GameMode = GetWorld()->GetAuthGameMode<ABaseballGameMode>();
	if (GameMode)
	{
		GameMode->ServerRPCBroadcastChatMessage(InChatMessageString);
	}
}

void ABaseballPlayerController::DisplayUserId(const FString& PlayerId)
{
	UE_LOG(LogTemp, Warning, TEXT("SetPlayerId %s"), *PlayerId);
	if (IsValid(BaseballUserWidgetInstance))
	{
		BaseballUserWidgetInstance->SetIdText(FText::FromString(PlayerId));
	}
}

void ABaseballPlayerController::CreateUserWidget()
{
	if (BaseballUserWidgetInstance == nullptr && IsLocalPlayerController())
	{
		if (IsValid(BaseballUserWidgetClass))
		{
			BaseballUserWidgetInstance = CreateWidget<UBaseballUserWidget>(this, BaseballUserWidgetClass);
			if (IsValid(BaseballUserWidgetInstance))
			{
				BaseballUserWidgetInstance->AddToViewport();
			}
		}
	}
}

void ABaseballPlayerController::DisplayTimer(const int32 NewTime)
{
	if (IsValid(BaseballUserWidgetInstance))
	{
		BaseballUserWidgetInstance->SetTimerText(FText::AsNumber(NewTime));
	}
}

void ABaseballPlayerController::DisplayTurn(const FString& PlayerId)
{
	if (IsValid(BaseballUserWidgetInstance))
	{
		BaseballUserWidgetInstance->SetTurnText(FText::FromString(PlayerId));

		ABaseballPlayerState* MyPlayerState = GetPlayerState<ABaseballPlayerState>();
		// 자신의 턴이라면
		if (MyPlayerState->UserId == PlayerId)
		{
			BaseballUserWidgetInstance->SetTurnTextColor(true);
			BaseballUserWidgetInstance->SetEditableBoxReadOnly(true);
		}
		else
		{
			BaseballUserWidgetInstance->SetTurnTextColor(false);
			BaseballUserWidgetInstance->SetEditableBoxReadOnly(false);
			BaseballUserWidgetInstance->SetTurnImage(MyPlayerState->RemainingTurn);
		}
	}
}

void ABaseballPlayerController::DisplayResult(const FString& ChatMessage)
{
	if (IsValid(BaseballUserWidgetInstance))
	{
		BaseballUserWidgetInstance->SetResultText(FText::FromString(ChatMessage));
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, ChatMessage);
}
