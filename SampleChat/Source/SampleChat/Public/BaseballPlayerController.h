// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseballPlayerController.generated.h"

class UBaseballUserWidget;
/**
 * 
 */
UCLASS()
class SAMPLECHAT_API ABaseballPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBaseballUserWidget> BaseballUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UBaseballUserWidget> BaseballUserWidgetInstance;

	FString ChatMessageString;

public:
	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCReceiveChatMessage(const FString& InChatMessageString);

	UFUNCTION(Server, Reliable)
	void ServerRPCSendChatMessage(const FString& InChatMessageString);

	void CreateUserWidget();
	void DisplayUserId(const FString& PlayerId);
	void DisplayTimer(const int32 NewTime);
	void DisplayTurn(const FString& PlayerId);
protected:
	void DisplayResult(const FString& ChatMessage);
};
