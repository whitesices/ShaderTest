#pragma once
#include "ShaderParameterUtils.h"
//引入渲染硬件接口
#include "RHIStaticStates.h"
//引入shader头文件
#include "Shader.h"
//引入全局Shader头文件
#include "GlobalShader.h"
//引入RenderGrapg头文件
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
//引入shader参数头文件
#include "ShaderParameterStruct.h"
#include "ShaderParameterMAcros.h"
#include "UniformBuffer.h"
#include "RHICommandList.h"
//引入数据驱动shader平台信息的头文件
#include "DataDrivenShaderPlatformInfo.h"
//引入动态RHI数组的资源
#include "Containers/DynamicRHIResourceArray.h"
//引入像素工具
#include "PixelShaderUtils.h"
#include "SimpleShaderLibraryType.h"

//构建相应的渲染函数
void DrawToRenderTarget_RenderThread(
	FRDGBuilder& RDGBuilder,
	const FTestShaderParameters& DrawParameters,
	FRDGTextureSRVRef outputTexture
);
