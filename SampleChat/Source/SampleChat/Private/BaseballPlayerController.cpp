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
		SetPlayerId(MyPlayerState->UserId);
	}
}

void ABaseballPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;
	
	ServerRPCSendChatMessage(InChatMessageString);
}

void ABaseballPlayerController::ClientRPCReceiveChatMessage_Implementation(const FString& InChatMessageString)
{
	DisplayChatMessage(InChatMessageString);
}

void ABaseballPlayerController::ServerRPCSendChatMessage_Implementation(const FString& InChatMessageString)
{
	ABaseballGameMode* GameMode = GetWorld()->GetAuthGameMode<ABaseballGameMode>();
	if (GameMode)
	{
		GameMode->ServerRPCBroadcastChatMessage(InChatMessageString);
	}
}

void ABaseballPlayerController::SetPlayerId(const FString& PlayerId)
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

void ABaseballPlayerController::DisplayChatMessage(const FString& ChatMessage)
{
	if (IsValid(BaseballUserWidgetInstance))
	{
		BaseballUserWidgetInstance->SetResultText(FText::FromString(ChatMessage));
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, ChatMessage);
}
