#include "../include/revDrawCommand.h"
revDrawCommandStorage::revDrawCommandStorage(uint32 capacity, uint32 threadNum)
{
    emptyBlock.resize(threadNum);
    emptyBlock.assign(threadNum, 0);
    storage = new revDrawCommand*[threadNum];
    for(int i = 0; i < threadNum; ++i){
        storage[i] = new revDrawCommand[capacity];
        for(int j = 0; j < capacity; ++j){
            storage[i][j].next = j+1;
        }
    }
}

revDrawCommand* revDrawCommandStorage::Borrow(uint32 threadId)
{
    revDrawCommand* ret = &storage[threadId][emptyBlock[threadId]];
    emptyBlock[threadId] = ret->next;
    return ret;
}

void revDrawCommandStorage::Return(revDrawCommand* command, uint32 threadId)
{
    command->next = emptyBlock[threadId];
    emptyBlock[threadId] = (reinterpret_cast<uint64>(command) - reinterpret_cast<uint64>(storage[threadId])) / sizeof(revDrawCommand);
}