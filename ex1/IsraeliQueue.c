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
}
*Node;

struct IsraeliQueue_t
{
    int friendship_threshold;
    int rivalry_threshold;
    FriendshipFunction* friendship_functions;
    ComparisonFunction comparison_function;
    Node head;
};

IsraeliQueue IsraeliQueueCreate(FriendshipFunction* friendship_functions, ComparisonFunction comparison_function, int friendship_threshold, int rivalry_threshold)
{
    IsraeliQueue new_queue = malloc(sizeof(*new_queue));
    if (new_queue == NULL)
    {
        return NULL;
    }
    new_queue->friendship_threshold = friendship_threshold;
    new_queue->rivalry_threshold = rivalry_threshold;
    new_queue->friendship_functions = friendship_functions;
    new_queue->comparison_function = comparison_function;
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

IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue queue, FriendshipFunction friendship_function)
{
    if (queue == NULL || friendship_function == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    FriendshipFunction* new_friendship_functions = malloc(sizeof(*new_friendship_functions) * (IsraeliQueueSize(queue) + 1));
    if (new_friendship_functions == NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    int i = 0;
    while (queue->friendship_functions[i] != NULL)
    {
        new_friendship_functions[i] = queue->friendship_functions[i];
        i++;
    }
    new_friendship_functions[i] = friendship_function;
    new_friendship_functions[i + 1] = NULL;
    free(queue->friendship_functions);
    queue->friendship_functions = new_friendship_functions;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue queue, int friendship_threshold)
{
    if (queue == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    queue->friendship_threshold = friendship_threshold;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue queue, int rivalry_threshold)
{
    if (queue == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    queue->rivalry_threshold = rivalry_threshold;
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
        if (queue->comparison_function(current->item, item) == 0)
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
    if (q == NULL)
    {
        return NULL;
    }
    IsraeliQueue new_queue = IsraeliQueueCreate(q->friendship_functions, q->comparison_function, q->friendship_threshold, q->rivalry_threshold);
    if (new_queue == NULL)
    {
        return NULL;
    }
    Node current = q->head;
    while (current != NULL)
    {
        void* item_copy = current->item;
        IsraeliQueueEnqueue(new_queue, item_copy);
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
        friendship_threshold_sum += current_queue->friendship_threshold;
        rivalry_threshold_sum += current_queue->rivalry_threshold;
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
    //TODO : Implement
}
