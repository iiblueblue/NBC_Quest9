// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseballGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeUpdated, int32, NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnUpdated, FString, Id);
/**
 * 
 */
UCLASS()
class SAMPLECHAT_API ABaseballGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(ReplicatedUsing= OnRep_RemainingTime, BlueprintReadOnly, Category = "Turn")
	int32 RemainingTime;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentTurnPlayer, BlueprintReadOnly, Category = "Turn")
	APlayerState* CurrentTurnPlayer;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnTimeUpdated OnTimeUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnTurnUpdated OnTurnUpdated;

	UFUNCTION()
	void OnRep_RemainingTime();

	UFUNCTION()
	void OnRep_CurrentTurnPlayer();

	void UpdateTime(int32 NewTime);
	void UpdateTurn(FString Id);

	UFUNCTION(BlueprintCallable)
	void SetNextTurn();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
