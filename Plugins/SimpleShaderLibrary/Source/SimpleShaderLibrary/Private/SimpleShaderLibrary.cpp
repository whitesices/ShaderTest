// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleShaderLibrary.h"
//引入插件管理类
#include "Interfaces/IPluginManager.h"

#include "RenderGraphBuilder.h"
//
#include "SimpleShaderCore.h"

#define LOCTEXT_NAMESPACE "FSimpleShaderLibraryModule"

void FSimpleShaderLibraryModule::StartRender()
{
	//开始渲染进行判断绑定
	if (!GEngine->GetPreRenderDelegate().IsBoundToObject(this))
	{
		GEngine->GetPreRenderDelegateEx().AddRaw(this, &FSimpleShaderLibraryModule::Render);
	}
}

void FSimpleShaderLibraryModule::EndRender()
{
	if (!GEngine)
	{
		return;
	}
	//移除渲染信息
	GEngine->GetPreRenderDelegateEx().RemoveAll(this);
}

//渲染中
void FSimpleShaderLibraryModule::Render(FRDGBuilder& RDGBuilder)
{
	//渲染时候检查是否在渲染线程里面
	check(IsInRenderingThread());

	//检查shader是否发生变化
	if (!bDirt)
	{
		return;
	}
	
	bDirt = false;

	//加锁
	InputLock.Lock();
	FTestShaderParameters InputParameters = CachedShaderParameters;
	InputLock.Unlock();

	//判断RenderTarget是否为空
	if (!InputParameters.RenderTarget)
	{
		return;
	}

	//创建渲染对象的描述
	FRDGTextureDesc OutputDesc = FRDGTextureDesc::Create2D(
		InputParameters.GetRenderTargetSize(),
		EPixelFormat::PF_R32_UINT,
		FClearValueBinding::None,
		ETextureCreateFlags::RenderTargetable | ETextureCreateFlags::UAV | ETextureCreateFlags::ShaderResource
	);

	//输出相应的纹理
	FRDGTextureRef OutputTexture = RDGBuilder.CreateTexture(OutputDesc , TEXT("Hello_Text"), ERDGTextureFlags::None );

	//绘制
	DrawToRenderTarget_RenderThread(RDGBuilder , InputParameters , RDGBuilder.CreateSRV(OutputTexture) );
}

void FSimpleShaderLibraryModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	//获取当前shader存放的路径//获取的是相对路径
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("SimpleShaderLibrary"))->GetBaseDir(),TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/SimpleShaderLibrary"), PluginShaderDir);
	
	//先初始化为flase
	bDirt = false;
}

void FSimpleShaderLibraryModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	//模块结束卸载render
	EndRender();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleShaderLibraryModule, SimpleShaderLibrary)