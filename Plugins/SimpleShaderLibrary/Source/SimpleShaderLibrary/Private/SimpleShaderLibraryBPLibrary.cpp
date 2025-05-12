// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleShaderLibraryBPLibrary.h"
#include "SimpleShaderLibrary.h"

USimpleShaderLibraryBPLibrary::USimpleShaderLibraryBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float USimpleShaderLibraryBPLibrary::SimpleShaderLibrarySampleFunction(float Param)
{
	return -1;
}

