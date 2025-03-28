// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "BaseballPlayerController.h"

ABaseballPlayerState::ABaseballPlayerState()
{
    bReplicates = true;
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
        PC->SetPlayerId(UserId);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("OnRep_UserId: PlayerController is NULL!"));
    }
    //UE_LOG(LogTemp, Warning, TEXT("In Client UserId Changed: %s"), *UserId);
}

void ABaseballPlayerState::SetUserId(const FString& NewUserId)
{
    if (HasAuthority())  // 서버에서 실행할 때만 동작
    {
        UserId = NewUserId;
        //OnRep_PlayerId();
        //UE_LOG(LogTemp, Log, TEXT("In Server UserId Changed: %s"), *UserId);
    }
    else
    {
        //UE_LOG(LogTemp, Error, TEXT("[Client] Try to Change UserId!"));
    }
}

void ABaseballPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // UserId를 복제 (Replicate)
    DOREPLIFETIME(ABaseballPlayerState, UserId);
}
