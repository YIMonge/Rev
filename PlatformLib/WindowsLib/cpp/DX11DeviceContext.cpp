#include "../include/DX11DeviceContext.h"
#include "../include/Window.h"


#if defined(_USE_DIRECTX11)

bool DX11DeviceContext::Create(Window* window, const GraphicsDesc& desc)
{
	unsigned int createDeviceFlags = 0;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0, sizeof(sd));

	switch (desc.bufferType)
	{
	case GraphicsDesc::BUFFERTYPE::TRIPLE_BUFFER:
		sd.BufferCount = 2;
		break;
	default:
		sd.BufferCount = 1;
		break;
	}

	switch (desc.bufferFormat)
	{
	case GraphicsDesc::BUFFERFORMAT::R8B8G8A8:
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		break;

	/*
	case GraphicsDesc::BUFFERFORMAT::R8B8G8A8:
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		break;
	*/
	default:
		return false;
		break;
	}

	sd.BufferDesc.Width = window->GetWidth();
	sd.BufferDesc.Height = window->GetHeight();
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = window->GetHWnd();

	// not use default AA
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.Windowed = !desc.useFullscreen;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL current_level;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		featureLevels,
		sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		&current_level,
		&device_context
		);
	if (FAILED(hr)) {
		// TODO:Assert
		return false;
	}
	return true;
}

#endif