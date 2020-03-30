#ifndef __DX12RENDERER_H__
#define __DX12RENDERER_H__

#include "revRenderer.h"
#include "DX12DeviceContext.h"
#include "DX12SwapChain.h"
#include "DX12RenderInfo.h"
#include "DX12Shader.h"
#include "DX12Buffer.h"
#include "DX12Texture.h"
#include "DX12TextureView.h"
#include "revArray.h"


#ifdef _USE_DIRECTX12

class Window;

class DX12Renderer : public revRenderer
{
public:
	virtual void StartUp(Window* window, const GraphicsDesc& desc);
	virtual void ShutDown();

	virtual void Render();

	virtual void Clear(bool clear_color, bool clear_depth, const revColor& fill_color) {}
	virtual void SwapBuffers() {}

	virtual void SetBlendFunc(BLEND_FUNC func) {}
	virtual void SetAlphaTest(ALPHA_TEST func, float value) {}

private:
	bool IntialzieForApp();

private:
	Window* main_window;
	DX12DeviceContext deviceContext;
	DX12SwapChain swapChain;
	DX12RenderInfo renderInfo;

	D3D12_VIEWPORT viewport;
	D3D12_RECT rectScissor;

	// Shader for test 
	DX12Shader vertexShader;
	DX12Shader fragmentShader;

	// vertex buffer for test
	DX12VertexBuffer vertexBuffer;
	DX12Texture texture;
	DX12TextureView textureView;
};

#endif

#endif
