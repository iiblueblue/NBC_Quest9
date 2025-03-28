// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BaseballPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLECHAT_API ABaseballPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABaseballPlayerState();

	UPROPERTY(ReplicatedUsing = OnRep_UserId, BlueprintReadWrite, Category="Player Info")
	FString UserId;

	UFUNCTION()
	void OnRep_UserId();

	UFUNCTION(BlueprintCallable)
	void SetUserId(const FString& NewUserId);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
