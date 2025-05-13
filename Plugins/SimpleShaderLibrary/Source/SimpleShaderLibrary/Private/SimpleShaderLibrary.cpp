// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleShaderLibrary.h"
//引入插件管理类
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FSimpleShaderLibraryModule"

void FSimpleShaderLibraryModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	//获取当前shader存放的路径//获取的是相对路径
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("SimpleShaderLibrary"))->GetBaseDir(),TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/SimpleShaderLibrary"), PluginShaderDir);
	
}

void FSimpleShaderLibraryModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleShaderLibraryModule, SimpleShaderLibrary)