#ifdef _USE_DIRECTX12

#include "DX12RootSignature.h"
#include "Log.h"

bool DX12RootSignature::Create(revDevice* device, const revDescriptorBindingDesc& desc)
{
	// setup root parameter 
	revArray<revArray<D3D12_DESCRIPTOR_RANGE1>> ranges(desc.GetDescriptorSetLayoutCount());
	uint32 elementCount = desc.GetDescriptorSetLayoutCount();
	revArray<D3D12_ROOT_PARAMETER1> rootParams(elementCount);
	byteOffsets.resize(elementCount);
	uint32 byteOffset = 0;

	uint32 index = 0;
	for (uint32 i = 0; i < desc.GetDescriptorSetLayoutCount(); ++i) {
		const auto& descriptorSetLayout = desc.GetDescriptorSetLayout(i);
		uint32 rangeCount = descriptorSetLayout.GetRangeCount();
		ranges[i].resize(rangeCount);
		rootParams[index].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParams[index].ShaderVisibility = ConvertToDXShaderVisibility(descriptorSetLayout.GetShaderVisiblity());
		rootParams[index].DescriptorTable.NumDescriptorRanges = rangeCount;
		rootParams[index].DescriptorTable.pDescriptorRanges = ranges[i].data();

		for (uint32 j = 0; j < rangeCount; ++j) {
			const auto& range = descriptorSetLayout.GetRange(j);
			ranges[i][j].BaseShaderRegister = range.registerIndex;
			ranges[i][j].NumDescriptors = range.count;
			ranges[i][j].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
			ranges[i][j].RangeType = ConverToDXDescriptorType(descriptorSetLayout.GetDescriptorType());
			ranges[i][j].RegisterSpace = range.space;
			ranges[i][j].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE; // D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
		}
		byteOffsets[index] = byteOffset;
		byteOffset += 8;
		++index;
	}

	if (index != elementCount) {
		NATIVE_LOGE("not match root parameter count and root parameter desc coun. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}

	// create root signature 
	D3D12_VERSIONED_ROOT_SIGNATURE_DESC signatureDesc;
	memset(&signatureDesc, 0, sizeof(signatureDesc));
	signatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;

	signatureDesc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	if (desc.isLocal()) signatureDesc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
	
	signatureDesc.Desc_1_1.pParameters = rootParams.data();
	signatureDesc.Desc_1_1.NumParameters = static_cast<uint32>(rootParams.size());
	signatureDesc.Desc_1_1.pStaticSamplers = nullptr;
	signatureDesc.Desc_1_1.NumStaticSamplers = 0;


	ID3DBlob* signature;
	ID3DBlob* error;
	HRESULT hr = D3D12SerializeVersionedRootSignature(&signatureDesc,
		&signature,
		&error);
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to serialize to root signature. detail is : ", reinterpret_cast<const char*>(error->GetBufferPointer()));
		return false;
	}

	if (sizeInBytes > sizeof(uint32)* D3D12_MAX_ROOT_COST) {
		NATIVE_LOGE("size in bytes over root signature max cost. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}

	hr = device->GetDevice()->CreateRootSignature(0,
		signature->GetBufferPointer(),
		signature->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)
	);
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to create root signature");
		return false;
	}

	if (signature) signature->Release();
	if (error) error->Release();
	return true;
}


void DX12RootSignature::Apply(revGraphicsCommandList& commandList, bool isGraphics)
{
	auto& dxCommandList = commandList.GetList();
	if (isGraphics) dxCommandList->SetGraphicsRootSignature(rootSignature);
	else dxCommandList->SetComputeRootSignature(rootSignature);
}

#endif
