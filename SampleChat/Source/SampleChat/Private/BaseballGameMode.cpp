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
		GS->RemainingTime = 15; // 제한 시간 초기화
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
		if (IsAllTurnsUsed())
		{
			GameOver();
		}
		else
		{
			// 타이머 초기화
			if (GetWorld()->GetTimerManager().IsTimerActive(TurnTimerHandle))
			{
				GetWorld()->GetTimerManager().ClearTimer(TurnTimerHandle);
			}
			StartTurnTimer();
		}
		
		GS->SetNextTurn();
		ABaseballPlayerState* PS = Cast<ABaseballPlayerState>(GS->CurrentTurnPlayer);
		PS->SetRemainingTurn(PS->RemainingTurn - 1);
		
	}
}

void ABaseballGameMode::ServerRPCBroadcastChatMessage_Implementation(const FString& ChatMessage)
{
	TArray<int32> Msg = UMyBlueprintFunctionLibrary::ConvertStringToArray(ChatMessage);
	FString Result = CompareNumbers(Answer, Msg);
	if (Result == "Answer")
	{
		GameSuccess();
	}
	else
	{
		BroadcastingChatMessage(Result);
	}
}

TArray<int32> ABaseballGameMode::GenerateRandomThreeDigitNumber()
{
	TArray<int32> AvailableNumber = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	TArray<int32> RandomNumber;

	for (int32 i = 0; i < 3; i++)
	{
		int32 Index = FMath::RandRange(0, AvailableNumber.Num() - 1);
		RandomNumber.Add(AvailableNumber[Index]);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("answer : %d"), AvailableNumber[Index]));

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

void ABaseballGameMode::GameOver()
{
	// 타이머 초기화
	if (GetWorld()->GetTimerManager().IsTimerActive(TurnTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(TurnTimerHandle);
	}
	BroadcastingGameResult(false);
}

void ABaseballGameMode::GameSuccess()
{
	// 타이머 초기화
	if (GetWorld()->GetTimerManager().IsTimerActive(TurnTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(TurnTimerHandle);
	}
	BroadcastingGameResult(true);
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
			PC->DisplayUserId(PlayerID);
		}
	}

	if (GetNumPlayers() >= 2)
	{
		GetWorld()->GetTimerManager().SetTimer(GameStartTimerHandle, this, &ABaseballGameMode::ChangeTurn, 2.0f, false);
	}
}

void ABaseballGameMode::BeginPlay()
{
	GenerateRandomThreeDigitNumber();
}

bool ABaseballGameMode::IsAllTurnsUsed()
{
	for (TActorIterator<ABaseballPlayerController> It(GetWorld()); It; ++It)
	{
		ABaseballPlayerController* PlayerController = *It;
		if (IsValid(PlayerController))
		{
			ABaseballPlayerState* PS = Cast<ABaseballPlayerState>(PlayerController->PlayerState);
			if (PS->RemainingTurn > 0)
			{
				return false;
			}
		}
	}
	return true;
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

	if (ChatMessage != "DRAW")
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("NotDraw")));
		ChangeTurn();
	}
	
}

void ABaseballGameMode::BroadcastingGameResult(bool IsSuccess)
{
	for (TActorIterator<ABaseballPlayerController> It(GetWorld()); It; ++It)
	{
		ABaseballPlayerController* PlayerController = *It;
		if (IsValid(PlayerController))
		{
			if (IsSuccess)
			{
				ABaseballPlayerState* PS = Cast<ABaseballPlayerState>(PlayerController->PlayerState);
				if (PS)
				{
					ABaseballGameState* GS = GetGameState<ABaseballGameState>();
					if (GS)
					{
						if (PS == GS->CurrentTurnPlayer)
						{
							PlayerController->ClientRPCReceiveChatMessage("WIN");
						}
						else
						{
							PlayerController->ClientRPCReceiveChatMessage("LOSE");
						}
					}
				}
			}
			else
			{
				PlayerController->ClientRPCReceiveChatMessage("DRAW");
			}
		}
	}
}
