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
    virtual bool Init() { return true; }

    // Update on multi thread
    virtual void Update() {}
    virtual void LateUpdate() {}

    // update on single thread
    virtual void SyncUpdate() {}

    bool IsActive() const { return isActive; }
    void SetActive(bool flag) { isActive = flag; }

    bool IsDead() const { return isDead; }
    void Destroy() { isDead = true; }

    bool IsUseUpdate() const { return usingUpdate; }
    bool IsUseLateUpdate() const { return usingLateUpdate; }
private:
    // unique id for recognition object
    uint32 id;
    // active status of the object
    bool isActive;
    // is object already dead 
    bool isDead;
    // the object use update function
    bool usingUpdate;
    // the object use late update function
    bool usingLateUpdate;
    // the object use sync update function
    bool usingSyncUpdate;    
};

#endif
