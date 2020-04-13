#ifndef __DX12RENDERER_H__
#define __DX12RENDERER_H__

#include "revRenderer.h"
#include "DX12Device.h"
#include "DX12SwapChain.h"
#include "DX12RenderInfo.h"
#include "DX12Shader.h"
#include "DX12Buffer.h"
#include "DX12Texture.h"
#include "DX12TextureView.h"
#include "DX12RootSignature.h"
#include "DX12CommandList.h"
#include "DX12PipelineState.h"
#include "revMaterial.h"
#include "revArray.h"


#ifdef _USE_DIRECTX12

class Window;

class DX12Renderer : public revRenderer
{
public:
	virtual void StartUp(Window* window, const GraphicsDesc& desc);
	virtual void ShutDown();

	virtual void Render();

private:
	bool IntialzieForApp();

	void LoadAssets();
	void LoadPipeline();


private:
	Window* main_window;
	DX12Device device;
	DX12SwapChain swapChain;
	DX12RenderInfo renderInfo;
	DX12RootSignature rootSiganture;
	DX12PipelineState pipelineState;

	D3D12_VIEWPORT viewport;
	D3D12_RECT rectScissor;


	// resources for test
	DX12Shader vertexShader;
	DX12Shader fragmentShader;
	revMaterial mat;
	DX12VertexBuffer vertexBuffer;
	DX12Texture texture;
	DX12TextureView textureView;
};

#endif

#endif
