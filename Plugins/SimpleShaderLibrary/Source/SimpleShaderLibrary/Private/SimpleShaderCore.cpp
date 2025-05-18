#include "SimpleShaderCore.h"
//#include "SimpleShaderLibraryType.h"


//声明顶点着色器
class FSimplePassThroughVS : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FSimplePassThroughVS);//声明全局shader
	SHADER_USE_PARAMETER_STRUCT(FSimplePassThroughVS, FGlobalShader);
	//定义shader参数
	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		//在此处参数可以写进shader

	END_SHADER_PARAMETER_STRUCT()

public:
	//定义预编译函数
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return true;
	}

};

//像素着色器  主要是坐标转换，逐像素处理
class FPixelShaderExamplePS :public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FPixelShaderExamplePS);//声明全局shader
	SHADER_USE_PARAMETER_STRUCT(FPixelShaderExamplePS, FGlobalShader);
	//定义shader参数
	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		//在此处参数可以写进shader
	END_SHADER_PARAMETER_STRUCT()

public:
	static bool shouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		//DX12已经支持sm6
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
};

//定义全局的shader参数
BEGIN_SHADER_PARAMETER_STRUCT(FDrawExampleParameters, )
	SHADER_PARAMETER_STRUCT_INCLUDE(FSimplePassThroughVS::FParameters, VS)
	SHADER_PARAMETER_STRUCT_INCLUDE(FPixelShaderExamplePS::FParameters, PS)
	RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

IMPLEMENT_GLOBAL_SHADER(FSimplePassThroughVS, "/SimpleShaderLibrary/Private/VSPS.usf", "VSMain", SF_Vertex);
IMPLEMENT_GLOBAL_SHADER(FPixelShaderExamplePS, "/SimpleShaderLibrary/Private/VSPS.usf", "PSMain", SF_Pixel);

//绘制出来
class FSimpleScreenVertexBuffer : public FVertexBuffer
{
public:
	virtual void InitRHI( FRHICommandListBase& RHICmdList ) override
	{
		//声明一个资源数组来存储顶点
		TResourceArray< FFilterVertex, VERTEXBUFFER_ALIGNMENT > Vertices;

		//这里需要做预分配否则会崩溃
		Vertices.SetNumUninitialized(6);

		//视口顶点数据
		Vertices[0].Position = FVector4f(-1, 1, 0, 1);
		Vertices[0].UV = FVector2f(0, 0);

		Vertices[1].Position = FVector4f(1, 1, 0, 1);
		Vertices[1].UV = FVector2f(1, 0);

		Vertices[2].Position = FVector4f(-1, -1, 0, 1);
		Vertices[2].UV = FVector2f(0, 1);

		Vertices[3].Position = FVector4f(1, -1, 0, 1);
		Vertices[3].UV = FVector2f(1, 1);

		FRHIResourceCreateInfo CreateInfo(TEXT("Hello"), &Vertices);
		VertexBufferRHI = RHICmdList.CreateVertexBuffer( Vertices.GetResourceDataSize() , BUF_Static , CreateInfo);
	}
};

//创建全局资源
TGlobalResource<FSimpleScreenVertexBuffer> GSimpleScreenVertexBuffer;

void DrawToRenderTarget_RenderThread(FRDGBuilder& RDGBuilder, const FTestShaderParameters& DrawParameters, FRDGTextureSRVRef outputTexture)
{
	//1.准备好PSO
	//2.绑定好shader
	//3.添加Path去执行
	//创建RenderTarget,通过获取资源来创建
	TRefCountPtr<IPooledRenderTarget> RenderTarget = CreateRenderTarget( DrawParameters.RenderTarget->GetResource()->GetTexture2DRHI() , TEXT("Draw my REnderTarget"));
	//注册成额外的资产
	FRDGTextureRef RenderTargetTexture = RDGBuilder.RegisterExternalTexture(RenderTarget);

	//进行分配
	FDrawExampleParameters* ShaderParams = RDGBuilder.AllocParameters<FDrawExampleParameters>();
	//加载到RenderTarget,进行相应的绑定
	ShaderParams->RenderTargets[0] = FRenderTargetBinding(RenderTargetTexture , ERenderTargetLoadAction::ELoad);

	//进行参数的设定
	auto ShaderMap = GetGlobalShaderMap( GMaxRHIFeatureLevel );
	TShaderMapRef<FSimplePassThroughVS> VertexShader(ShaderMap);
	TShaderMapRef<FPixelShaderExamplePS> PixelShader(ShaderMap);

	//添加通道
	RDGBuilder.AddPass(
		RDG_EVENT_NAME("TestWSLPass"),
		ShaderParams,
		ERDGPassFlags::Raster,
		[ShaderParams , VertexShader , PixelShader]( FRHICommandList& RHICmdList)
		{
			//先声明创建Pso
			FGraphicsPipelineStateInitializer GraphicsPSOInit;
			//应用PSO
			RHICmdList.ApplyCachedRenderTargets( GraphicsPSOInit );

			//对当前声明创建的Pso做初始化
			GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
			GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
			GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
			GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;//全局
			GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
			GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
			GraphicsPSOInit.PrimitiveType = PT_TriangleStrip;

			//进行相应设置
			SetGraphicsPipelineState( RHICmdList , GraphicsPSOInit , 0 );

			//设置相应的参数
			SetShaderParameters( RHICmdList , PixelShader , PixelShader.GetPixelShader() , ShaderParams->PS);
			SetShaderParameters( RHICmdList , VertexShader, VertexShader.GetVertexShader() , ShaderParams->VS);

			//shader 缓存通道
			RHICmdList.SetStreamSource( 0 , GSimpleScreenVertexBuffer.VertexBufferRHI , 0 );
			//进行绘制
			RHICmdList.DrawPrimitive(0, 2, 1);
		}
	);
}
