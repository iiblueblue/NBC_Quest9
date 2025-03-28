// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseballGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLECHAT_API ABaseballGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	TArray<int32> Answer;
	FTimerHandle TurnTimerHandle; // Ÿ�̸� �ڵ�
	FTimerHandle GameStartTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void StartTurnTimer();

	void DecreaseTime();
	void ChangeTurn();

	UFUNCTION(Server, Reliable)
	void ServerRPCBroadcastChatMessage(const FString& ChatMessage);

	UFUNCTION(BlueprintCallable, Category="Random")
	TArray<int32> GenerateRandomThreeDigitNumber();

	UFUNCTION(BlueprintCallable, Category = "Judge")
	FString CompareNumbers(TArray<int32> answer, TArray<int32> number);

	UFUNCTION(BlueprintCallable, Category="Game")
	void GameOver();
	void GameSuccess();

	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void BeginPlay() override;
	bool IsAllTurnsUsed();
protected:
	void BroadcastingChatMessage(const FString& ChatMessage);
	void BroadcastingGameResult(bool IsSuccess);

};
