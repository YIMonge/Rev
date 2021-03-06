#ifndef __DX12RENDERER_H__
#define __DX12RENDERER_H__
#ifdef _USE_DIRECTX12

#include "revRenderer.h"
#include "DX12Device.h"
#include "DX12SwapChain.h"
#include "DX12Shader.h"
#include "DX12Buffer.h"
#include "DX12Texture.h"
#include "DX12TextureView.h"
#include "DX12RootSignature.h"
#include "DX12CommandList.h"
#include "DX12PipelineState.h"
#include "DX12DescriptorHeap.h"
#include "DX12Sampler.h"
#include "DX12VertexBufferView.h"
#include "DX12ConstantBufferView.h"
#include "DX12MeshRenderer.h"
#include "revMaterial.h"
#include "revArray.h"
#include "revModel.h"

class Window;

class DX12Renderer : public revRenderer
{
public:
	virtual void StartUp(Window* window, const GraphicsDesc& desc);
	virtual void ShutDown();

	virtual void Render();

	virtual revDevice* GetDevice() { return &device; }
private:
	bool IntialzieForApp();

	void LoadAssets();
	void LoadPipeline();

	void ExecuteCommand(revArray<revGraphicsCommandList>& lists);
	void ExecuteCommand(revGraphicsCommandList& list);

private:
	Window* main_window;
	DX12Device device;
	DX12SwapChain swapChain;
	DX12RootSignature rootSiganture;
	DX12PipelineState pipelineState;

	DX12DescriptorHeap cbufferHeap;
	DX12DescriptorHeap textureHeap;
	DX12DescriptorHeap samplerHeap;

	// resources for test
	DX12Shader vertexShader;
	DX12Shader fragmentShader;
	revMaterial mat;
	revModel model;
	DX12MeshRenderer meshRenderer;
	DX12Texture texture;
	DX12TextureView textureView;
	DX12Sampler sampler;

	DX12ConstantBuffer* constantBuffer;
	DX12ConstantBufferView constantBufferView;


};

#endif

#endif
