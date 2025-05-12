// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint Function Libraries/ArrayBlueprintFunctionLibrary.h"


void UArrayBlueprintFunctionLibrary::SetArrayElement(const TArray<int32>& TargetArray, const int32& NewItem,
	const int32 Index)
{
	//we should never hit this. It's simply to avoid the NoExport on the class. Call the Generic Function instead.
	check(0);
}

void UArrayBlueprintFunctionLibrary::ShiftArrayElementsLeft(const TArray<int32>& TargetArray,
	const int32& ValueToInsertAtEnd, const int32 ShiftAmount)
{
	check(0);
}

void UArrayBlueprintFunctionLibrary::ShiftArrayElementsRight(const TArray<int32>& TargetArray,
	const int32& ValueToInsertAtStart, const int32 ShiftAmount)
{
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

void UArrayBlueprintFunctionLibrary::GenericShiftArrayElementsLeft(void* TargetArray, const FArrayProperty* ArrayProp,
	const void* ValueToInsertAtEnd, int32 ShiftAmount)
{
	
	// Early exit if TargetArray is null
	if (!TargetArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetArray is null."));
		return;
	}

	//creates a helper class to manipulate the array, TargetArray is the address of the array, array prop holds the metadata of the array
	FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);

	const int32 ArraySize = ArrayHelper.Num();

	// Validate array size and shift amount
	if (ArraySize <= 0 || ShiftAmount <= 0 || ShiftAmount >= ArraySize)
	{
		UE_LOG(LogTemp, Warning, TEXT("Array is empty or shift amount is invalid."));
		return;
	}
	
	const int32 ShiftLimit = ArraySize - ShiftAmount;

	// Shift elements to the left
	for (int32 i = 0; i < ShiftLimit; ++i)
	{
		ArrayProp->Inner->CopyCompleteValue(ArrayHelper.GetRawPtr(i), ArrayHelper.GetRawPtr(i + ShiftAmount));
	}

	// Fill the remaining elements with the provided value
	for (int32 i = ShiftLimit; i < ArraySize; ++i)
	{
		ArrayProp->Inner->CopyCompleteValue(ArrayHelper.GetRawPtr(i), ValueToInsertAtEnd);
	}
}

void UArrayBlueprintFunctionLibrary::GenericShiftArrayElementsRight(void* TargetArray, const FArrayProperty* ArrayProp,
	const void* ValueToInsertAtStart, int32 ShiftAmount)
{
	// Early exit if TargetArray is null
	if (!TargetArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetArray is null."));
		return;
	}

	//creates a helper class to manipulate the array, TargetArray is the address of the array, array prop holds the metadata of the array
	FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);

	const int32 ArraySize = ArrayHelper.Num();

	// Validate array size and shift amount
	if (ArraySize <= 0 || ShiftAmount <= 0 || ShiftAmount >= ArraySize)
	{
		UE_LOG(LogTemp, Warning, TEXT("Array is empty or shift amount is invalid."));
		return;
	}

	for (int32 i = ArraySize-1; i >= ShiftAmount	; --i)
	{
		ArrayProp->Inner->CopyCompleteValue(ArrayHelper.GetRawPtr(i), ArrayHelper.GetRawPtr(i - ShiftAmount));
	}

	// Fill the remaining elements with the provided value
	for (int32 i = 0; i < ShiftAmount; ++i)
	{
		ArrayProp->Inner->CopyCompleteValue(ArrayHelper.GetRawPtr(i), ValueToInsertAtStart);
	}
}
