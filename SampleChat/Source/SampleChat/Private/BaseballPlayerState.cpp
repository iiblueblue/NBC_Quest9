// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "BaseballPlayerController.h"

ABaseballPlayerState::ABaseballPlayerState()
{
    bReplicates = true;
    RemainingTurn = 3;
}

void ABaseballPlayerState::OnRep_UserId()
{
    UE_LOG(LogTemp, Warning, TEXT("OnRep_UserId Triggered - New Value: %s"), *UserId);
    ABaseballPlayerController* PC = Cast<ABaseballPlayerController>(GetOwner());  // 기존 방식 (잘못될 가능성 있음)
    if (!PC)
    {
        PC = Cast<ABaseballPlayerController>(GetWorld()->GetFirstPlayerController());  // 새로운 방식 (더 안정적)
    }

    if (IsValid(PC))
    {
        PC->DisplayUserId(UserId);
    }
}

void ABaseballPlayerState::SetUserId(const FString& NewUserId)
{
    if (HasAuthority())  // 서버에서 실행할 때만 동작
    {
        UserId = NewUserId;
    }
}

void ABaseballPlayerState::SetRemainingTurn(const int32 NewTurns)
{
    if (HasAuthority())  // 서버에서 실행할 때만 동작
    {
        RemainingTurn = NewTurns;
    }
}

void ABaseballPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // UserId를 복제 (Replicate)
    DOREPLIFETIME(ABaseballPlayerState, UserId);
    DOREPLIFETIME(ABaseballPlayerState, RemainingTurn);
}
