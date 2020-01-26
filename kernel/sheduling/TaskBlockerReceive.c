#include "sheduling/TaskBlockerReceive.h"
#include "tasking.h"

bool blocker_receive_can_unblock(TaskBlockerReceive *blocker, Task *task)
{
    __unused(task);

    return !fsnode_is_acquire(blocker->handle->node) && fsnode_can_receive(blocker->handle->node, blocker->handle);
}

void blocker_receive_unblock(TaskBlockerReceive *blocker, Task *task)
{
    fsnode_acquire_lock(blocker->handle->node, task->id);
}

TaskBlocker *blocker_receive_create(FsHandle *handle)
{
    TaskBlockerReceive *receive_blocker = __create(TaskBlockerReceive);

    receive_blocker->blocker = (TaskBlocker){
        (TaskBlockerCanUnblock)blocker_receive_can_unblock,
        (TaskBlockerUnblock)blocker_receive_unblock,
    };

    receive_blocker->handle = handle;

    return (TaskBlocker *)receive_blocker;
}