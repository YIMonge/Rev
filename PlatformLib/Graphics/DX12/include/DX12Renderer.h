#ifndef __DX12RENDERER_H__
#define __DX12RENDERER_H__
#ifdef _USE_DIRECTX12

#include "revRenderer.h"
#include "DX12Device.h"
#include "DX12Fence.h"
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
	DX12Renderer(){}
	virtual ~DX12Renderer(){}
	virtual void StartUp(Window* window, const GraphicsDesc& desc);
	virtual void ShutDown();

	virtual void Render();

	virtual revDevice* GetDevice() { return &device; }

	virtual void OpenGlobalCommandList();
	virtual void CloseGlobalCommandList();
	virtual void ExecuteGlobalCommandList();

	virtual void ExecuteCommand(revArray<revGraphicsCommandList>& lists);
	virtual void ExecuteCommand(revGraphicsCommandList& list);


	void TestCode();
private:	
	Window* window;
	DX12Device device;
	DX12Fence* fence;
	DX12SwapChain swapChain;
	DX12RootSignature rootSiganture;
	DX12PipelineState pipelineState;

	DX12DescriptorHeap* cbufferHeap;
	DX12DescriptorHeap* textureHeap;
	DX12DescriptorHeap* samplerHeap;
	
	// resources for test
	revShader* vertexShader;
	revShader* fragmentShader;
	revMaterial mat;
	revModel* model;
	DX12MeshRenderer meshRenderer;
	revTexture* texture;
	DX12TextureView* textureView;
	DX12Sampler sampler;

	DX12ConstantBuffer* constantBuffer;
	DX12ConstantBufferView* constantBufferView;

};

#endif

#endif
