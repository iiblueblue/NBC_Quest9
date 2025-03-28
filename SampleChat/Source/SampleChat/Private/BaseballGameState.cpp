// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballGameState.h"
#include "Net/UnrealNetwork.h"
#include "BaseballPlayerState.h"
#include "BaseballPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void ABaseballGameState::OnRep_RemainingTime()
{
	ABaseballPlayerController* PlayerController = Cast<ABaseballPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->DisplayTimer(RemainingTime);
	}
}

void ABaseballGameState::OnRep_CurrentTurnPlayer()
{
	if (!CurrentTurnPlayer)
	{
		return;
	}

	// APlayerState ĳ����
	ABaseballPlayerState* PlayerState = Cast<ABaseballPlayerState>(CurrentTurnPlayer);
	if (!PlayerState)
	{	
		return;
	}

	FString UserId = PlayerState->UserId;

	// UI ������Ʈ �̺�Ʈ ����
	ABaseballPlayerController* PlayerController = Cast<ABaseballPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->DisplayTurn(UserId);
	}
}

void ABaseballGameState::UpdateTime(int32 NewTime)
{
	if (HasAuthority())
	{
		RemainingTime = NewTime;
		OnRep_RemainingTime(); // ���������� UI ������Ʈ �̺�Ʈ ����
	}
}

void ABaseballGameState::SetNextTurn()
{
	if (HasAuthority())
	{
		if (PlayerArray.Num() < 2) return;

		APlayerState* FirstPlayerState = PlayerArray[0];
		APlayerState* SecondPlayerState = PlayerArray[1];

		APlayerState* FirstPlayer = FirstPlayerState;
		APlayerState* SecondPlayer = SecondPlayerState;

		if (!FirstPlayer || !SecondPlayer) return;

		CurrentTurnPlayer = (CurrentTurnPlayer == FirstPlayer) ? SecondPlayer : FirstPlayer;

		// �׽�Ʈ
		if (ABaseballPlayerState* TurnPS = Cast<ABaseballPlayerState>(CurrentTurnPlayer))
		{
			if (TurnPS->UserId.IsEmpty())
			{
				TurnPS->SetUserId(FString::Printf(TEXT("Player_%d"), TurnPS->GetUniqueID()));  // ������ ID ����
			}
		}

		OnRep_CurrentTurnPlayer();
	}
}

void ABaseballGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseballGameState, CurrentTurnPlayer);
	DOREPLIFETIME(ABaseballGameState, RemainingTime);
}
