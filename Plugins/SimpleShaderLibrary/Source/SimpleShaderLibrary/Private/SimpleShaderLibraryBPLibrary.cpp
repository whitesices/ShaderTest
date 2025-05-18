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

void USimpleShaderLibraryBPLibrary::startRender()
{
	//执行定义好的模块里相应的接口
	FSimpleShaderLibraryModule::Get().StartRender();
}

void USimpleShaderLibraryBPLibrary::UpdateShaderParam(const FTestShaderParameters& InParam)
{
	FSimpleShaderLibraryModule::Get().CachedShaderParameters = InParam;
	FSimpleShaderLibraryModule::Get().bDirt = true;
}

