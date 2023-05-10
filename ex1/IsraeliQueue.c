#include "IsraeliQueue.h"
#include "Node.h"
#include <stdio.h>
#include <stdlib.h>


void improveNode(IsraeliQueue queue, Node nodeToImprove);

// Creates a new IsraeliQueue with specified friendship functions, comparison function,
// friendship threshold, and rivalry threshold
IsraeliQueue IsraeliQueueCreate(FriendshipFunction* friendshipFunctions, ComparisonFunction comparisonFunction,
    int friendshipThreshold, int rivalryThreshold)
{
    IsraeliQueue newQueue = (IsraeliQueue)malloc(sizeof(*newQueue));
    if (newQueue == NULL) 
    {
        return NULL;
    }
    newQueue->friendshipThreshold = friendshipThreshold;
    newQueue->rivalryThreshold = rivalryThreshold;
    newQueue->comparisonFunction = comparisonFunction;
    int numOfFunctions = 0;
    while (friendshipFunctions[numOfFunctions])
    {
        numOfFunctions++;
    }
    FriendshipFunction* newFriendshipFunctions = (FriendshipFunction*)malloc(sizeof(*newFriendshipFunctions) * (numOfFunctions + 1));
    int function = 0;
    while (friendshipFunctions[function])
    {
        newFriendshipFunctions[function] = friendshipFunctions[function];
        function++;
    }
    newFriendshipFunctions[function] = NULL;
    newQueue->friendshipFunctions = newFriendshipFunctions;
    newQueue->tail = NULL;
    return newQueue;
}

// Frees memory allocated for a given IsraeliQueue and its nodes
void IsraeliQueueDestroy(IsraeliQueue queue)
{
    if (queue == NULL)
    {
        return;
    }
    Node current = queue->tail;
    while (current != NULL)
    {
        Node temp = current;
        current = current->next;
        free(temp);
    }
    free(queue->friendshipFunctions);
    free(queue);
}

// Returns the size of IsraeliQueue
int IsraeliQueueSize(IsraeliQueue queue)
{
    if (queue == NULL)
    {
        return 0;
    }
    int size = 0;
    Node current = queue->tail;
    while (current != NULL)
    {
        size++;
        current = current->next;
    }
    return size;
}

// Adds a friendship measure function to the IsraeliQueue
IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue queue, FriendshipFunction friendshipFunction)
{
    if (queue == NULL || friendshipFunction == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    FriendshipFunction* friendshipFunctions = queue->friendshipFunctions;
    int i = 0;
    while (friendshipFunctions[i])
    {
        i++;
    }
    FriendshipFunction* newFriendshipFunctions = (FriendshipFunction*)malloc(sizeof(*newFriendshipFunctions) * (i + 2));
    if (!newFriendshipFunctions)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    i = 0;
    while (friendshipFunctions[i])
    {
        newFriendshipFunctions[i] = friendshipFunctions[i];
        i++;
    }
    queue->friendshipFunctions = newFriendshipFunctions;
    queue->friendshipFunctions[i] = friendshipFunction;
    queue->friendshipFunctions[i + 1] = NULL;
    return ISRAELIQUEUE_SUCCESS;
}

// Updates the friendship threshold of the IsraeliQueue
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue queue, int newFriendshipThreshold)
{
    if (queue == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    queue->friendshipThreshold = newFriendshipThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

// Updates the rivalry threshold of the IsraeliQueue
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue queue, int newRivalryThreshold)
{
    if (queue == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    queue->rivalryThreshold = newRivalryThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

// Checks if the given IsraeliQueue contains the specified item
bool IsraeliQueueContains(IsraeliQueue queue, void* item)
{
    if (queue == NULL || item == NULL)
    {
        return false;
    }
    Node current = queue->tail;
    while (current != NULL)
    {
        if (queue->comparisonFunction(current->item, item) == 0)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Removes and returns the item at the head of the IsraeliQueue
void* IsraeliQueueDequeue(IsraeliQueue queue)
{
    if (queue == NULL || IsraeliQueueSize(queue) == 0)
    {
        return NULL;
    }
    Node before = NULL;
    Node current = queue->tail;
    while (current->next != NULL)
    {
        before = current;
        current = current->next;
    }
    void* item = current->item;
    if (before == NULL)
    {
        queue->tail = NULL;
    }
    else
    {
        before->next = NULL;
    }
    free(current);
    return item;
}

// Clones the given IsraeliQueue, returning a new IsraeliQueue with the same items
IsraeliQueue IsraeliQueueClone(IsraeliQueue queue)
{
    if (queue == NULL)
    {
        return NULL;
    }
    IsraeliQueue new_queue = IsraeliQueueCreate(queue->friendshipFunctions, queue->comparisonFunction,
        queue->friendshipThreshold, queue->rivalryThreshold);
    if (new_queue == NULL)
    {
        return NULL;
    }
    Node current = queue->tail;
    while (current != NULL)
    {
        void* itemCopy = current->item;
        IsraeliQueueError err = IsraeliQueueEnqueue(new_queue, itemCopy);
        if (err != ISRAELIQUEUE_SUCCESS)
        {
            IsraeliQueueDestroy(new_queue);
            return NULL;
        }
        current = current->next;
    }
    return new_queue;
}

// Merges an array of IsraeliQueues into a single IsraeliQueue
IsraeliQueue IsraeliQueueMerge(IsraeliQueue* qarr, ComparisonFunction compare_function)
{
    if (qarr == NULL)
    {
        return NULL; // Return NULL if qarr is NULL
    }
    int num_queues = 0;
    // Calculate the number of queues in qarr
    int merged_friendship_threshold_sum = 0;
    int merged_rivalry_threshold_sum = 0;
    for (; qarr[num_queues] != NULL; num_queues++)
    {
        merged_friendship_threshold_sum += qarr[num_queues]->friendshipThreshold;
        merged_rivalry_threshold_sum += abs(qarr[num_queues]->rivalryThreshold);
    }
    // Return NULL if there are no queues in qarr
    if (num_queues == 0) return NULL;
    // Calculate the "membership dimensions" of the merged queues
    int merged_friendship_threshold = merged_friendship_threshold_sum / num_queues;
    int merged_rivalry_threshold = merged_rivalry_threshold_sum / num_queues;
    if (merged_rivalry_threshold_sum % num_queues != 0) {
        merged_rivalry_threshold++; // Round up if necessary
    }
    // Create a new merged array of frienshipFunction
    int newNumberOfFriendshipFunc = 0;
    for (int i = 0, j = 0; i < num_queues; i++)
    {
        while (qarr[i]->friendshipFunctions[j])
        {
            newNumberOfFriendshipFunc++;
            j++;
        }
    }
    FriendshipFunction* newTab = (FriendshipFunction*)malloc((newNumberOfFriendshipFunc + 1) * sizeof(*newTab));
    int idx = 0;
    for (int i = 0; i < num_queues; i++)
    {
        FriendshipFunction* cur = qarr[i]->friendshipFunctions;
        while (*cur)
        {
            newTab[idx] = *cur;
            idx++;
            cur++;
        }
    }
    newTab[newNumberOfFriendshipFunc] = NULL;
    IsraeliQueue merged_queue = IsraeliQueueCreate(newTab, compare_function, merged_friendship_threshold, merged_rivalry_threshold);
    if (merged_queue == NULL)
    {
        return NULL; // Return NULL if failed to create new queue
    }
    // Enqueue items from each queue in round-robin order
    bool all_queues_empty = false;
    while (!all_queues_empty)
    {
        all_queues_empty = true;
        for (int i = 0; i < num_queues; i++)
        {
            if (IsraeliQueueSize(qarr[i]) > 0)
            {
                all_queues_empty = false;
                void* person = IsraeliQueueDequeue(qarr[i]);
                if (person != NULL)
                {
                    IsraeliQueueEnqueue(merged_queue, person);
                }
            }
        }
    }
    return merged_queue;
}

// Adds an item to the tail of the IsraeliQueue and improves the node's position if necessary
IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue queue, void* item)
{
    if (queue == NULL || item == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    Node newNode = (Node)malloc(sizeof(*newNode));
    if (newNode == NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    newNode->item = item;
    newNode->friendCount = 0;
    newNode->rivalCount = 0;
    newNode->next = queue->tail;
    queue->tail = newNode;
    improveNode(queue, newNode);
    return ISRAELIQUEUE_SUCCESS;
}

// Inserts the given node after the specified friend node in the IsraeliQueue
void IsraeliQueueInsertNode(IsraeliQueue queue, Node friend, Node item)
{
    if (queue == NULL || item == NULL)
    {
        return;
    }
    if (queue->tail == friend)
    {
        item->next = queue->tail;
        queue->tail = item;
        return;
    }

    Node previous = queue->tail;
    Node current = queue->tail->next;
    while (current && current != friend)
    {
        previous = current;
        current = current->next;
    }
    item->next = current;
    previous->next = item;
}

// Removes the specified node from the IsraeliQueue
void IsraeliQueueRemoveNode(IsraeliQueue queue, Node item)
{
    if (queue == NULL || queue->tail == NULL)
    {
        return;
    }
    if (item == queue->tail)
    {
        queue->tail = item->next;
        return;
    }
    else
    {
        Node prev = queue->tail;
        Node curr = queue->tail->next;
        while (item != curr)
        {
            prev = curr;
            curr = curr->next;
        }
        prev->next = curr->next;
    }
}

// Determines if the given items are friends based on the IsraeliQueue's friendship functions
bool isFriend(IsraeliQueue queue, void* item1, void* item2)
{
    if (!queue)
    {
        return false;
    }
    int i = 0;
    while (queue->friendshipFunctions[i] != NULL)
    {
        if (queue->friendshipFunctions[i](item1, item2) > queue->friendshipThreshold)
        {
            return true;
        }
        i++;
    }
    return false;
}

// Determines if the given items are enemies based on the IsraeliQueue's friendship functions
bool isEnemy(IsraeliQueue queue, void* item1, void* item2)
{
    if (!queue || !*(queue->friendshipFunctions))
        return false;
    int i = 0, friendshipThresholdAverage = 0;
    while (queue->friendshipFunctions[i] != NULL)
    {
        friendshipThresholdAverage += queue->friendshipFunctions[i](item1, item2);
        if (isFriend(queue, item1, item2))
        {
            return false;
        }
        i++;
    }
    if (friendshipThresholdAverage / i < queue->rivalryThreshold)
    {
        return true;
    }
    return false;
}

// Finds the farthest enemy node before the specified friend node in the IsraeliQueue
Node FindFarthestEnemyBeforeFriend(IsraeliQueue queue, Node toImprove, Node friend)
{
    Node curr = toImprove->next;
    if (!curr)
        return NULL;
    Node farthestEnemyBeforeFriend = NULL;
    while (curr->next && curr != friend)
    {
        if (isEnemy(queue, toImprove->item, curr->item) && curr->rivalCount < RIVAL_QUOTA)
        {
            farthestEnemyBeforeFriend = curr;
        }
        curr = curr->next;
    }
    return farthestEnemyBeforeFriend;
}

// Finds the farthest friend node before the specified enemy node in the IsraeliQueue
Node FindFarthestFriendBeforeEnemy(IsraeliQueue queue, Node toImprove, Node enemy)
{
    Node lastFriendBeforeEnemy = queue->tail;
    Node curr = lastFriendBeforeEnemy->next;
    while (curr != NULL && curr != enemy)
    {
        printf("");
        if ((isFriend(queue, toImprove->item, curr->item)) && (curr->friendCount < FRIEND_QUOTA))
        {
            lastFriendBeforeEnemy = curr;
        }
        curr = curr->next;
    }
    return lastFriendBeforeEnemy;
}

// Determines if the enemy node appears before the friend node in the IsraeliQueue
bool enemyIsBeforeFriend(Node enemy, Node friend)
{
    Node curr = enemy;
    while (curr)
    {
        if (curr == friend)
            return true;
        curr = curr->next;
    }
    return false;
}

// Improves the position of the specified node in the IsraeliQueue
void improveNode(IsraeliQueue queue, Node toImprove)
{
    if (toImprove->next == NULL)
        return;
    Node enemy = NULL;
    Node friend;
    while (1)
    {
        friend = FindFarthestFriendBeforeEnemy(queue, toImprove, enemy);
        enemy = FindFarthestEnemyBeforeFriend(queue, toImprove, friend);
        if (enemyIsBeforeFriend(enemy, friend))
        {
            enemy->rivalCount++;
            continue;
        }
        else
        {
            if (friend == toImprove || !friend)
                return;
            friend->friendCount++;
            IsraeliQueueRemoveNode(queue, toImprove);
            IsraeliQueueInsertNode(queue, friend, toImprove);
            break;
        }
    }
}

// Improves the position of all nodes in the IsraeliQueue
IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue queue)
{
    if (!queue)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    Node curr = queue->tail;
    if (curr == NULL)
    {
        return ISRAELIQUEUE_SUCCESS;
    }
    IsraeliQueue clonedQueue = IsraeliQueueClone(queue);
    Node toImprove = clonedQueue->tail;
    while (toImprove != NULL)
    {
        Node nodeInRealQ = queue->tail;
        while (queue->comparisonFunction(toImprove->item, nodeInRealQ->item))
        {
            nodeInRealQ = nodeInRealQ->next;
        }
        improveNode(queue, nodeInRealQ);
        toImprove = toImprove->next;
    }
    IsraeliQueueDestroy(clonedQueue);
    return ISRAELIQUEUE_SUCCESS;
}

// Returns the position of the specified item in the IsraeliQueue
int getPosition(IsraeliQueue queue, void* item)
{
    int size = IsraeliQueueSize(queue);
    Node current = queue->tail;
    int position = size;
    while (current)
    {
        if (current->item == item)
        {
            break;
        }
        current = current->next;
        position--;
    }
    return position;
