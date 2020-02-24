#pragma once

struct Widget;

typedef enum
{
    EVENT_CHILD_ADDED,
    EVENT_CHILD_REMOVED,

    __EVENT_TYPE_COUNT,
} EventType;

typedef struct Event
{
    struct Widget *sender;
    EventType type;
    bool accepted;
} Event;