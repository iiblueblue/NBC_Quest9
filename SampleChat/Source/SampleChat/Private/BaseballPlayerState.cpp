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
    ABaseballPlayerController* PC = Cast<ABaseballPlayerController>(GetOwner());  // ���� ��� (�߸��� ���ɼ� ����)
    if (!PC)
    {
        PC = Cast<ABaseballPlayerController>(GetWorld()->GetFirstPlayerController());  // ���ο� ��� (�� ������)
    }

    if (IsValid(PC))
    {
        PC->DisplayUserId(UserId);
    }
}

void ABaseballPlayerState::SetUserId(const FString& NewUserId)
{
    if (HasAuthority())  // �������� ������ ���� ����
    {
        UserId = NewUserId;
    }
}

void ABaseballPlayerState::SetRemainingTurn(const int32 NewTurns)
{
    if (HasAuthority())  // �������� ������ ���� ����
    {
        RemainingTurn = NewTurns;
    }
}

void ABaseballPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // UserId�� ���� (Replicate)
    DOREPLIFETIME(ABaseballPlayerState, UserId);
    DOREPLIFETIME(ABaseballPlayerState, RemainingTurn);
}
