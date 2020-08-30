#ifndef __REVUPLOADABLERESOURCE_H__
#define __REVUPLOADABLERESOURCE_H__

#include "revResource.h"

class revDevice;

/// <summary>
/// the resource can upload tp VRAM
/// </summary>
class revUploadableResource : public revResource
{
public:
	revUploadableResource(){}
	virtual ~revUploadableResource() {}

	/// <summary>
	/// Upload to VRAM
	/// </summary>
	/// <param name="device">graphics device</param>
	/// <returns>if failed to upload return false</returns>
	virtual bool Upload(revDevice* device) = 0;
	/// <summary 
	/// notify coplete upload 
	/// </summary>
	virtual void OnUploaded(){}
};

#endif
