#include "../ex1/ex1/IsraeliQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 

int main()
{
    printf("Hello World\n");
    return 0;
}

typedef struct node_t
{
    void* item;
    int friend_count;
    int rival_count;
    struct node* next;
}*Node;

struct IsraeliQueue_t
{
    int friendshipThreshold;
    int rivalryThreshold;
    FriendshipFunction* friendshipFunctions;
    ComparisonFunction comparisonFunction;
    Node head;
};


IsraeliQueue IsraeliQueueCreate(FriendshipFunction* friendshipFunctions, ComparisonFunction comparisonFunction, int friendshipThreshold, int rivalryThreshold)
{
    IsraeliQueue new_queue = malloc(sizeof(*new_queue));
    if (new_queue == NULL)
    {
        return NULL;
    }
    new_queue->friendshipThreshold = friendshipThreshold;
    new_queue->rivalryThreshold = rivalryThreshold;
    new_queue->friendshipFunctions = friendshipFunctions;
    new_queue->comparisonFunction = comparisonFunction;
    new_queue->head = NULL;

    return new_queue;
}


void IsraeliQueueDestroy(IsraeliQueue queue)
{
    if (queue == NULL)
    {
        return;
    }
    Node current = queue->head;
    while (current != NULL)
    {
        Node temp = current;
        current = current->next;
        free(temp);
    }
    free(queue);
}

int IsraeliQueueSize(IsraeliQueue queue)
{
    if (queue == NULL)
    {
        return 0;
    }
    int size = 0;
    Node current = queue->head;
    while (current != NULL)
    {
        size++;
        current = current->next;
    }
    return size;
}

IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue queue, FriendshipFunction friendshipFunction)
{
    if (queue == NULL || friendshipFunction == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    FriendshipFunction* new_friendshipFunctions = malloc(sizeof(*new_friendshipFunctions) * (IsraeliQueueSize(queue) + 1));
    if (new_friendshipFunctions == NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    int i = 0;
    while (queue->friendshipFunctions[i] != NULL)
    {
        new_friendshipFunctions[i] = queue->friendshipFunctions[i];
        i++;
    }
    new_friendshipFunctions[i] = friendshipFunction;
    new_friendshipFunctions[i + 1] = NULL;
    free(queue->friendshipFunctions);
    queue->friendshipFunctions = new_friendshipFunctions;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue queue, int friendshipThreshold)
{
    if (queue == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    queue->friendshipThreshold = friendshipThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue queue, int rivalryThreshold)
{
    if (queue == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    queue->rivalryThreshold = rivalryThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

bool IsraeliQueueContains(IsraeliQueue queue, void* item)
{
    if (queue == NULL || item == NULL)
    {
        return false;
    }
    Node current = queue->head;
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

void* IsraeliQueueDequeue(IsraeliQueue queue) {
    if (queue == NULL)
    {
        return NULL;
    }
    Node current = queue->head;
    if (current == NULL)
    {
        return NULL;
    }
    while (current->next != NULL)
    {
		current = current->next;
	}
    void* item = current->item;
	free(current);
	return item;
}

IsraeliQueue IsraeliQueueClone(IsraeliQueue queue) {
    IsraeliQueue new_queue = IsraeliQueueCreate(queue->friendshipFunctions, queue->comparisonFunction, queue->friendshipThreshold, queue->rivalryThreshold);
    if (new_queue == NULL)
    {
		return NULL;
	}
    Node current = queue->head;
    while (current != NULL)
    {
		IsraeliQueueEnqueue(new_queue, current->item);
		current = current->next;
	}
	return new_queue;
}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue queue, void* item) {
    if(queue == NULL || item == NULL) 
    { 
        return ISRAELIQUEUE_BAD_PARAM; 
    }
    Node new_node = malloc(sizeof(*new_node));
    if (new_node == NULL)
    {
		return ISRAELIQUEUE_ALLOC_FAILED;
	}
    new_node->item = item;
	new_node->friend_count = 0;
	new_node->rival_count = 0;
	new_node->next = NULL;
	Node current = queue->head;
    if (current == NULL)
    {
		queue->head = new_node;
		return ISRAELIQUEUE_SUCCESS;
	}

    //Use the Friendship Functions to know where to insert the new node
}

IsraeliQueue IsraeliQueueMerge(IsraeliQueue* queuePtr, ComparisonFunction comparisonFunction) {
    //TODO : Implement
} 
 
IsraeliQueueError IsraeliQueueImprovePosition(IsraeliQueue queue) {
    //TOO : Implement
}