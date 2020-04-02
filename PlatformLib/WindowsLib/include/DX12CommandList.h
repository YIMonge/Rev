#ifndef __DX12COMMANDLIST_H__
#define __DX12COMMANDLIST_H__

#include "revGraphicsCommandList.h"

class DX12CommandList : public revGraphicsCommandList
{
public:
	DX12CommandList() {}
	virtual ~DX12CommandList(){}

	virtual bool Create(const revDevice& device);


};

#endif
