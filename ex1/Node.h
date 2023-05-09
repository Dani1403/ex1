#pragma once
#ifndef NODE_H
#define NODE_H

#include "IsraeliQueue.h"
typedef struct node_t
{
    void* item;
    int friendCount;
    int rivalCount;
    struct node_t* next;
} *Node;

struct IsraeliQueue_t
{
    int friendshipThreshold;
    int rivalryThreshold;
    FriendshipFunction* friendshipFunctions;
    ComparisonFunction comparisonFunction;
    Node tail;
};

#endif