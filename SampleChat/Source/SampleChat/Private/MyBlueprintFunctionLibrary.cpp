// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

TArray<int32> UMyBlueprintFunctionLibrary::ConvertStringToArray(const FString& InputString)
{
	TArray<int32> Result;

	for (int i = 0; i < InputString.Len(); i++)
	{
		FString CharStr = InputString.Mid(i, 1);
		if (CharStr == "/") continue;
		int32 Num = FCString::Atoi(*CharStr);

		
		if (Num > 0 && Num <= 9)
		{
			Result.Add(Num);
		}
		else
		{
			Result.Add(0);
		}
	}

	return Result;
}

bool UMyBlueprintFunctionLibrary::bIsResponse(FString InputString)
{
	if (InputString.Mid(0, 1) == "/")
	{
		return true;
	}
	else
	{
		return false;
	}
}
