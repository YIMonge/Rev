#ifndef __DX12COMMANDLIST_H__
#define __DX12COMMANDLIST_H__

#include "revGraphicsCommandList.h"

class DX12CommandList : public revGraphicsCommandList
{
public:
	DX12CommandList() {}
	virtual ~DX12CommandList(){}

	virtual bool Create(revDevice* device, revGraphicsPipeline* pipeline);
	virtual void Open();
	virtual void Close();
	void SetPipelineState(revGraphicsPipeline& pipeline);

private:
};

#endif
