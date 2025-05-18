// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "SimpleShaderLibraryType.generated.h"

//暴露给蓝图
USTRUCT(BlueprintType)
struct FTestShaderParameters
{
	GENERATED_USTRUCT_BODY()
	//新建一个renderTarget
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShaderParameters")
	UTextureRenderTarget2D* RenderTarget;

	FTestShaderParameters()
	{
		CachedRenderTargetSize = FIntPoint(1920, 1080);
	}
	FTestShaderParameters(UTextureRenderTarget2D* InRenderTarget) : RenderTarget(InRenderTarget)
	{
		//
		CachedRenderTargetSize = RenderTarget ? FIntPoint(RenderTarget->SizeX, RenderTarget->SizeY) : FIntPoint::ZeroValue;
	}

	//返回size
	FIntPoint GetRenderTargetSize() const
	{
		return CachedRenderTargetSize;
	}

protected:
	//缓存尺寸
	FIntPoint CachedRenderTargetSize;
};
