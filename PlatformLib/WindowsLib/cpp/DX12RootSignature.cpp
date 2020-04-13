#include "DX12RootSignature.h"

struct RootSignatureParam
{
	revArray<D3D12_ROOT_PARAMETER1> rootParams;
	revArray<revArray<D3D12_DESCRIPTOR_RANGE1>> ranges;
	uint32 sizeInBytes;
	revArray<uint32> offsets;
	bool isLocal;
};

bool DX12RootSignature::Create(revDevice* device)
{
	this->device = device;
	auto dxDevice = device->GetDevice();
	RootSignatureParam param;


	//-------------------------------------------------
	// TODO: desc to param
	CD3DX12_DESCRIPTOR_RANGE1 descriptorRange;
	descriptorRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	param.ranges.push_back(revArray<D3D12_DESCRIPTOR_RANGE1>(1, descriptorRange));
	
	CD3DX12_ROOT_PARAMETER1 rootParam;
	rootParam.InitAsDescriptorTable(1, &descriptorRange, D3D12_SHADER_VISIBILITY_PIXEL);
	param.rootParams.push_back(rootParam);


	//--------------------------------------------------

	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
	if (FAILED(dxDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData)))) {
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
	}

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC signatureDesc;
	signatureDesc.Init_1_1(
		param.rootParams.size(), param.rootParams.data(),	// D3D12_ROOT_PARAMETER1
		0, nullptr,											// Static Sampler
		param.isLocal ? D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE : D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);


	ID3DBlob* signature;
	ID3DBlob* error;
	HRESULT hr = D3DX12SerializeVersionedRootSignature(&signatureDesc,
		featureData.HighestVersion,
		&signature,
		&error);
	if (FAILED(hr)) {
		return false;
	}
	
	if (param.sizeInBytes > sizeof(uint32) * D3D12_MAX_ROOT_COST) {
		return false;
	}

	hr = dxDevice->CreateRootSignature(0,
		signature->GetBufferPointer(), 
		signature->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)
	);
	if (FAILED(hr)) {
		return false;
	}

	signature->Release();
	error->Release();
	return true;
}

void DX12RootSignature::Apply(revGraphicsCommandList& commandList, bool isGraphics)
{
	if (isGraphics) commandList.GetList()->SetGraphicsRootSignature(rootSignature);
	else commandList.GetList()->SetComputeRootSignature(rootSignature);
}

