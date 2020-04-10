#ifndef __DX12PIPELINESTATE_H__
#define __DX12PIPELINESTATE_H__

#include "DX12Device.h"
#include "revMaterial.h"
#include "DX12RootSignature.h"

class DX12PipelnieState
{
public:
	DX12PipelnieState(){}
	virtual ~DX12PipelnieState(){}

	bool Create(revDevice* device, const revMaterial& material, const DX12RootSignature& rootSignature);


private:
	revDevice* device;
};


#endif
