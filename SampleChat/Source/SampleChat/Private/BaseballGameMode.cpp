// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballGameMode.h"
#include "BaseballGameState.h"
#include "BaseballPlayerController.h"
#include "EngineUtils.h"
#include "BaseballPlayerState.h"
#include "MyBlueprintFunctionLibrary.h"

void ABaseballGameMode::StartTurnTimer()
{
	ABaseballGameState* GS = GetGameState<ABaseballGameState>();
	if (GS)
	{
		GS->RemainingTime = 10; // 제한 시간 초기화
	}

	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ABaseballGameMode::DecreaseTime, 1.0f, true);
}

void ABaseballGameMode::DecreaseTime()
{
	ABaseballGameState* GS = GetGameState<ABaseballGameState>();
	if (GS)
	{
		GS->UpdateTime(GS->RemainingTime - 1);

		if (GS->RemainingTime <= 0)
		{
			ChangeTurn();
		}
	}
}

void ABaseballGameMode::ChangeTurn()
{
	ABaseballGameState* GS = GetGameState<ABaseballGameState>();
	if (GS)
	{
		GS->SetNextTurn();
		StartTurnTimer();
	}
}

void ABaseballGameMode::ServerRPCBroadcastChatMessage_Implementation(const FString& ChatMessage)
{
	TArray<int32> Msg = UMyBlueprintFunctionLibrary::ConvertStringToArray(ChatMessage);
	BroadcastingChatMessage(CompareNumbers(Answer, Msg));
}

TArray<int32> ABaseballGameMode::GenerateRandomThreeDigitNumber()
{
	TArray<int32> AvailableNumber = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	TArray<int32> RandomNumber;

	for (int32 i = 0; i < 3; i++)
	{
		int32 Index = FMath::RandRange(0, AvailableNumber.Num() - 1);
		RandomNumber.Add(AvailableNumber[Index]);
		AvailableNumber.RemoveAt(Index);
	}
	Answer = RandomNumber;
	return RandomNumber;
}

FString ABaseballGameMode::CompareNumbers(TArray<int32> answer, TArray<int32> number)
{
	int32 Strikes = 0;
	int32 Balls = 0;

	// 형식이 맞지 않다면 OUT
	if (answer.Contains(0) || answer.Num() != 3)
	{
		return "OUT";
	}

	for (int i = 0; i < 3; i++)
	{
		if (answer[i] == number[i])
		{
			Strikes++;
		}
		else if (number.Contains(answer[i]))
		{
			Balls++;
		}
	}

	if (Strikes == 3)
	{
		return "Answer";
	}
	else if (Strikes == 0 && Balls == 0)
	{
		return "OUT";
	}
	else
	{
		return FString::Printf(TEXT("%dS%dB"), Strikes, Balls);
	}
}

void ABaseballGameMode::EndGame()
{

}

void ABaseballGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	ABaseballPlayerController* PC = Cast<ABaseballPlayerController>(NewPlayer);
	if (PC)
	{
		ABaseballPlayerState* PS = Cast<ABaseballPlayerState>(PC->PlayerState);
		if (PS)
		{
			FString PlayerID = PC->IsLocalController() ? TEXT("Host") : TEXT("Guest");

			PS->SetUserId(PlayerID);
		}
	}
}

void ABaseballGameMode::BroadcastingChatMessage(const FString& ChatMessage)
{
	for (TActorIterator<ABaseballPlayerController> It(GetWorld()); It; ++It)
	{
		ABaseballPlayerController* PlayerController = *It;
		if (IsValid(PlayerController))
		{
			PlayerController->ClientRPCReceiveChatMessage(ChatMessage);
		}
	}
}
