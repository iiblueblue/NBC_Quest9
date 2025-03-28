// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLECHAT_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Convert")
	static TArray<int32> ConvertStringToArray(const FString& InputString);

	UFUNCTION(BlueprintCallable, Category = "Response")
	static bool bIsResponse(FString InputString);
};
