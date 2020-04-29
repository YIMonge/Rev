#include "DX12RootSignature.h"
#include "Log.h"


void RootSignatureDesc::AddMaterial(const revMaterial& material)
{
	const revShader* vertexShader = material.GetShader(SHADER_TYPE::VERTX);
	const revShader* fragmentShader = material.GetShader(SHADER_TYPE::FRAGMENT);
	AddShader(vertexShader);
	AddShader(fragmentShader);
	/*
	revArray<DescriptorSetLayoutDesc> descriptorSets;
	revArray<RootConstantDesc> rootConstants;
	*/
}

void RootSignatureDesc::AddShader(const revShader* const shader)
{
	auto& textureBindings = shader->GetTextureBindings();

	// TODO: constants buffer 
	if (!textureBindings.empty()) {
		DescriptorSetLayoutDesc setDesc;
		setDesc.SetShaderVisiblity(SHADER_VISIBILITY::PIXEL);
		for (uint32 i = 0; i < textureBindings.size(); ++i) {
			DescriptorSetLayoutDesc::Range range;
			range.type = DESCRIPTOR_TYPE::TEXTURE_SHADER_RESOURCE_VIEW;
			range.registerIndex = textureBindings[i].GetRegisterIndex();
			range.count = textureBindings[i].GetCount();
			range.space = textureBindings[i].GetSpace();
			setDesc.AddRange(range);
		}
		descriptorSets.push_back(setDesc);
	}

	auto& samplerBindings = shader->GetSamplerBindings();
	if (!samplerBindings.empty()) {
		DescriptorSetLayoutDesc setDesc;
		setDesc.SetShaderVisiblity(SHADER_VISIBILITY::PIXEL);
		for (uint32 i = 0; i < samplerBindings.size(); ++i) {
			DescriptorSetLayoutDesc::Range range;
			range.type = DESCRIPTOR_TYPE::SAMPLER;
			range.registerIndex = samplerBindings[i].GetRegisterIndex();
			range.count = samplerBindings[i].GetCount();
			range.space = samplerBindings[i].GetSpace();
			setDesc.AddRange(range);
		}
		descriptorSets.push_back(setDesc);
	}
}


bool DX12RootSignature::Create(revDevice* device)
{
	struct  RootSignatureParam
	{
		revArray<revArray<D3D12_DESCRIPTOR_RANGE1>> ranges;
		uint32 sizeInBytes;
		revArray<uint32> offsets;
		revArray<D3D12_ROOT_PARAMETER1> rootParams;
		bool isLocal;
	};

	this->device = device;
	auto dxDevice = device->GetDevice();
	RootSignatureParam param;
	param.sizeInBytes = 0;
	param.isLocal = false;


	//-------------------------------------------------
	// TODO: desc to param
	CD3DX12_DESCRIPTOR_RANGE1 descriptorRange, samplerRange;
	descriptorRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	samplerRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0, 0);
	param.ranges.push_back(revArray<D3D12_DESCRIPTOR_RANGE1>(1, descriptorRange));
	param.ranges.push_back(revArray<D3D12_DESCRIPTOR_RANGE1>(1, samplerRange));
	
	CD3DX12_ROOT_PARAMETER1 rootParam, samplerParam;
	rootParam.InitAsDescriptorTable(1, &descriptorRange, D3D12_SHADER_VISIBILITY_PIXEL);
	samplerParam.InitAsDescriptorTable(1, &samplerRange, D3D12_SHADER_VISIBILITY_PIXEL);
	param.rootParams.push_back(rootParam);
	param.rootParams.push_back(samplerParam);

	//--------------------------------------------------

	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
	if (FAILED(dxDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData)))) {
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
	}

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC signatureDesc;
	signatureDesc.Init_1_1(
		param.rootParams.size(), param.rootParams.data(),	// D3D12_ROOT_PARAMETER1
		0, nullptr,							// Static Sampler
		param.isLocal ? D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE : D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);


	ID3DBlob* signature;
	ID3DBlob* error;
	HRESULT hr = D3DX12SerializeVersionedRootSignature(&signatureDesc,
		featureData.HighestVersion,
		&signature,
		&error);
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to serialize to root signature. detail is : %s. File[%s], line[%d]", reinterpret_cast<const char*>(error->GetBufferPointer()), __FILE__, __LINE__);
		return false;
	}
	
	if (param.sizeInBytes > sizeof(uint32) * D3D12_MAX_ROOT_COST) {
		NATIVE_LOGE("size in bytes over root signature max cost. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}

	hr = dxDevice->CreateRootSignature(0,
		signature->GetBufferPointer(), 
		signature->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)
	);
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to create root signature. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}

	if(signature) signature->Release();
	if( error) error->Release();
	return true;
}


bool DX12RootSignature::Create(revDevice* device, const RootSignatureDesc& desc)
{
	// setup root parameter 
	revArray<revArray<D3D12_DESCRIPTOR_RANGE1>> ranges(desc.GetDescriptorSetLayoutCount());
	uint32 elementCount = desc.GetDescriptorSetLayoutCount() + desc.GetRootConstantDescCount() + desc.GetRootDescriptorDescCount();
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
			ranges[i][j].RangeType = ConverToDXDescriptorType(range.type);
			ranges[i][j].RegisterSpace = range.space;
			ranges[i][j].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
		}
		byteOffsets[index] = byteOffset;
		byteOffset += 8;
		++index;
	}

	for (uint32 i = 0; i < desc.GetRootDescriptorDescCount(); ++i) {
		const auto& rootDesc = desc.GetRootDescriptorDesc(i);
		switch (rootDesc.type) {
		case DESCRIPTOR_TYPE::RAW_BUFFER_SHADER_RESOURCE_VIEW:
		case DESCRIPTOR_TYPE::TYPED_BUFFER_SHADER_RESOURCE_VIEW:
		case DESCRIPTOR_TYPE::STRUCTURED_BUFFER_SHADER_RESOURCE_VIEW:
			rootParams[index].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
			break;
		case DESCRIPTOR_TYPE::RAW_BUFFER_UNORDERED_RESOURCE_VIEW:
		case DESCRIPTOR_TYPE::TYPED_BUFFER_UNORDERED_RESOURCE_VIEW:
		case DESCRIPTOR_TYPE::STRUCTURED_BUFFER_UNORDERED_RESOURCE_VIEW:
			rootParams[index].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
			break;
		}
		rootParams[index].Descriptor.RegisterSpace = rootDesc.spaceIndex;
		rootParams[index].Descriptor.ShaderRegister = rootDesc.registerIndex;
		rootParams[index].Descriptor.Flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE;
		rootParams[index].ShaderVisibility = ConvertToDXShaderVisibility(rootDesc.visibility);

		byteOffsets[index] = byteOffset;
		byteOffset += 8;
		++index;
	}

	for (uint32 i = 0; i < desc.GetRootConstantDescCount(); ++i) {
		const auto& rootConst = desc.GetRootConstantDesc(i);
		rootParams[index].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParams[index].Constants.Num32BitValues = rootConst.count;
		rootParams[index].Constants.RegisterSpace = rootConst.spaceIndex;
		rootParams[index].Constants.ShaderRegister = rootConst.registerIndex;
		rootParams[index].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		byteOffsets[index] = byteOffset;
		byteOffset += 4 * rootConst.count;
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

	return true;
}


void DX12RootSignature::Apply(revGraphicsCommandList& commandList, bool isGraphics)
{
	auto& dxCommandList = commandList.GetList();
	if (isGraphics) dxCommandList->SetGraphicsRootSignature(rootSignature);
	else dxCommandList->SetComputeRootSignature(rootSignature);
}

