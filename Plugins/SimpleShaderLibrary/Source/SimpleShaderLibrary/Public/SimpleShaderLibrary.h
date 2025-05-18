// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#include "SimpleShaderLibraryType.h"

class FSimpleShaderLibraryModule : public IModuleInterface
{
	friend class USimpleShaderLibraryBPLibrary;

public:
	//插件内部调用模块加载函数
	static inline FSimpleShaderLibraryModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FSimpleShaderLibraryModule>("SimpleShaderLibrary");
	}

	//判断是否有效该插件
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("SimpleShaderLibrary");
	}

	//开关渲染
	void StartRender();
	void EndRender();

	//主渲染
	void Render(class FRDGBuilder& RDGBuilder);


	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


	

protected:
	//当前自定义的shader信息,缓存shader
	FTestShaderParameters CachedShaderParameters;
	//枷锁进行多线程管理
	FCriticalSection InputLock;
	//判断是否发生变化因为不能一直更新shader , 
	bool bDirt;
};
