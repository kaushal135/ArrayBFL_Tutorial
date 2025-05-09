// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ArrayBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARRAYTUTORIAL_API UArrayBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//Step 1: Create and define the UFunction we want to use
	
	UFUNCTION(BlueprintCallable, CustomThunk, meta=(
		DisplayName = "Set",
		CompactNodeTitle = "SET",
		ArrayParm = "TargetArray",
		ArrayTypeDependentParams = "NewItem",
		AutoCreateRefTerm = "NewItem",
		Category = "MyProject|Array"
	))
	static void SetArrayElement(const TArray<int32>& TargetArray, const int32& NewItem, const int32 Index);

	//Step 2: Create Native functions that will be called by the CustomThunk. This reads off property address, and calls the appropriate native handler
	static void GenericSetArrayElement(void* TargetArray, const FArrayProperty* ArrayProp,
	                                   const void* NewItem, const int32 Index);

	//Step 3: create an exec function that will be called by the custom thunk
	DECLARE_FUNCTION(execSetArrayElement)
	{
		//initialize the most recent property to null
		Stack.MostRecentProperty = nullptr;
	
		//step through the compiled array property
		Stack.StepCompiledIn<FArrayProperty>(nullptr);
	
		//get the address of the most recent property
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
	
		//cast the most recent property to an FArrayProperty
		const FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
	
		//if the property is not an array, mark the context as failed and return
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;;
		}
	
		//since NewItem might not really be an int, step the stack manually
		const FProperty* InnerProp = ArrayProperty->Inner;
	
		//calculate the total size of the property by multiplying the element size by the array dimension
		//(if the property is not an array, the array dimension is 1)
		const int32 PropertySize = InnerProp->ElementSize * InnerProp->ArrayDim;
	
		//Allocate temporary stack memory for the property using the calculated size
		void* StorageSpace = FMemory_Alloca(PropertySize);
	
		//Initialize the allocated memory with the default value of the property type
		InnerProp->InitializeValue(StorageSpace);
	
		//Reset the most recent property address and container to ensure a clean state
		Stack.MostRecentPropertyAddress = nullptr;
		Stack.MostRecentPropertyContainer = nullptr;
	
		//step through the compiled property stack to populate the allocated storage space
		Stack.StepCompiledIn<FProperty>(StorageSpace);
	
		//REad the index value from the stack
		P_GET_PROPERTY(FIntProperty, Index);
	
		//mark the end of the parameter reading
		P_FINISH;
	
		//begin native function call
		P_NATIVE_BEGIN;
	
			//call the generic function to set the array element
			GenericSetArrayElement(ArrayAddr, ArrayProperty, StorageSpace, Index);
	
			//End the native function call
		P_NATIVE_END;
	
		//Destroy the value in storage space and free the memory
		InnerProp->DestroyValue(StorageSpace);
	}

	
};
