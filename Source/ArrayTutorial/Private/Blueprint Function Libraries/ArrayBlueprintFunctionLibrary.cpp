// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint Function Libraries/ArrayBlueprintFunctionLibrary.h"


void UArrayBlueprintFunctionLibrary::SetArrayElement(const TArray<int32>& TargetArray, const int32& NewItem,
	const int32 Index)
{
	//we should never hit this. It's simply to avoid the NoExport on the class. Call the Generic Function instead.
	check(0);
}

void UArrayBlueprintFunctionLibrary::GenericSetArrayElement(void* TargetArray, const FArrayProperty* ArrayProp,
	const void* NewItem, const int32 Index)
{
	// Early exit if TargetArray is null
	if (!TargetArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetArray is null."));
		return;
	}

	//creates a helper class to manipulate the array, TargetArray is the address of the array, array prop holds the metadata of the array
	FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);

	if (Index < 0 || Index >= ArrayHelper.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Index is invalid."));
		return;
	}
	
	ArrayProp->Inner->CopySingleValueToScriptVM(ArrayHelper.GetRawPtr(Index), NewItem);
}
