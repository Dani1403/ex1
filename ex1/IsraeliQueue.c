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
    int friendCount;
    int rivalCount;
    struct node* next;
}
*Node;

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
    FriendshipFunction* friendshipFunctions = queue->friendshipFunctions;
    int i = 0;
    while (friendshipFunctions[i]) {
        i++;
    }
    FriendshipFunction* newFriendshipFunctions = malloc((i + 2) * sizeof(*newFriendshipFunctions));
    if (!newFriendshipFunctions)
    {
		return ISRAELIQUEUE_ALLOC_FAILED;
	}
    i = 0;
    while (friendshipFunctions[i]) {
        newFriendshipFunctions[i] = friendshipFunctions[i];
    }
    newFriendshipFunctions[i + 1] = friendshipFunction;
    newFriendshipFunctions[i + 2] = NULL;
    queue->friendshipFunctions = newFriendshipFunctions;
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
    if (queue == NULL)
    {
        return NULL;
    }
    IsraeliQueue new_queue = IsraeliQueueCreate(queue->friendshipFunctions, queue->comparisonFunction, queue->friendshipThreshold, queue->rivalryThreshold);
    if (new_queue == NULL)
    {
        return NULL;
    }
    Node current = queue->head;
    while (current != NULL)
    {
        void* itemCopy = current->item;
        IsraeliQueueEnqueue(new_queue, itemCopy);

        current = current->next;
    }
    return new_queue;
}

IsraeliQueue IsraeliQueueMerge(IsraeliQueue *qarr, ComparisonFunction compare_function) {
    // Vérification des paramètres d'entrée
    if (qarr == NULL || compare_function == NULL) {
        return NULL;
    }

    // Initialisation des paramètres de la nouvelle liste
    int friendship_threshold_sum = 0;
    int rivalry_threshold_sum = 0;
    int num_queues = 0;

    // Parcours des queues pour calculer les paramètres
    IsraeliQueue current_queue = qarr[0];
    while (current_queue != NULL) {
        friendship_threshold_sum += current_queue->friendshipThreshold;
        rivalry_threshold_sum += current_queue->rivalryThreshold;
        num_queues++;

        current_queue = qarr[num_queues];
    }

    // Création de la nouvelle file d'attente
    IsraeliQueue new_queue = IsraeliQueueCreate(NULL, compare_function, friendship_threshold_sum / num_queues, rivalry_threshold_sum / num_queues);
    if (new_queue == NULL) {
        return NULL;
    }

    // Parcours des queues pour ajouter les items au nouveau queue
    for (int i = 0; i < num_queues; i++) {
        current_queue = qarr[i];
        Node current_node = current_queue->head;

        while (current_node != NULL) {
            IsraeliQueueError err = IsraeliQueueEnqueue(new_queue, current_node->item);
            if (err != ISRAELIQUEUE_SUCCESS) {
                // En cas d'erreur lors de l'ajout d'un item, on détruit le nouveau queue et on retourne NULL
                IsraeliQueueDestroy(new_queue);
                return NULL;
            }

            current_node = current_node->next;
        }
    }

    return new_queue;
}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue queue, void* item) {
    if (queue == NULL || item == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    Node new_node = malloc(sizeof(*new_node));
    if (new_node == NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    new_node->item = item;
    new_node->friendCount = 0;
    new_node->rivalCount = 0;
    new_node->next = NULL;
    Node current = queue->head;
    if (current == NULL)
    {
        queue->head = new_node;
        return ISRAELIQUEUE_SUCCESS;
    }
    int maxPosition = findMaxPosition(queue, item);
    insertNode(queue, new_node, maxPosition);
}

IsraeliQueueError IsraeliQueueImprovePosition(IsraeliQueue queue) {
    if (queue == NULL) {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    Node current = queue->head;
    while (current != NULL) {

    }
}

bool isFriend(FriendshipFunction* friendshipFunctions, int friendshipThreshold, void* item1, void* item2) {
    int i = 0;
    while (friendshipFunctions[i]) {
        if (friendshipFunctions[i](item1, item2) > friendshipThreshold) {
            return true;
        }
        i++;
    }
    return false;
}

bool isEnemy(FriendshipFunction* friendshipFunctions, int rivalryThreshold, int friendshipThreshold, void* item1, void* item2) {
    int i = 0, sum = 0;
    while (friendshipFunctions[i]) {
        sum += friendshipFunctions[i](item1, item2);
        if (friendshipFunctions[i](item1, item2) > friendshipThreshold) {
			return false;
		}
        i++;
    }
    if ((sum / i) < rivalryThreshold) {
		return true;
	}
    return false;
}

void insertNode(Node* head, Node node, int position) {
    if (position == 1) {
		node->next = *head;
		*head = node;
		return;
	}
	Node current = *head;
	int i = 1;
    while (i < position - 1) {
		current = current->next;
		i++;
	}
	node->next = current->next;
	current->next = node;
    current->friendCount++;
}

int findMaxPosition(IsraeliQueue queue, void* item) {
    Node current = queue->head;
    int position = 1, maxPosition = 1;
    while (current != NULL) {
        current = current->next;
        if (isEnemy(queue->friendshipFunctions, queue->rivalryThreshold, queue->friendshipThreshold, current->item, item) && (current->rivalCount < RIVAL_QUOTA)) {
            current->rivalCount++;
            break;
        }
        if (isFriend(queue->friendshipFunctions, queue->friendshipThreshold, current->item, item) && current->friendCount < FRIEND_QUOTA) {
            maxPosition = position;
        }
    }
}