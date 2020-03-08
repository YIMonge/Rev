#ifndef __REVGAMEOBJECT_H__
#define __REVGAMEOBJECT_H__

#include "revTypedef.h"
#include "revArray.h"

class revGameObject
{
public:
    revGameObject();
    virtual ~revGameObject();

    // is return true object has been initialized. if want to split initialize while frames should use false
    virtual bool Init(){ return true }

    // Update on multi thread
    virtual void Update() {}
    virtual void LateUpdate() {}

    // update on single thread
    virtual void SyncUpdate() {}

protected:
    // unique id for recognition object
    uint32 id;
    // update priority that is decided by dependencies
    uint32 priority;
    // object dependencies
    revArray<uint32> dependencies;
};

#endif
