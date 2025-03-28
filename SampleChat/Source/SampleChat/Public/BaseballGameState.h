// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseballGameState.generated.h"
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

	UFUNCTION()
	void OnRep_RemainingTime();

	UFUNCTION()
	void OnRep_CurrentTurnPlayer();

	void UpdateTime(int32 NewTime);

	UFUNCTION(BlueprintCallable)
	void SetNextTurn();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
