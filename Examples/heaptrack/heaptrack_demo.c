#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NODE_COUNT 10000

typedef struct Node {
    int id;
    char *data;
    struct Node *next;
} Node;

Node* create_node(int id, const char *text) {
    Node *node = malloc(sizeof(Node));
    if (!node) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    node->id = id;
    node->data = strdup(text); // allocate and copy string
    node->next = NULL;
    return node;
}

void free_list(Node *head) {
    while (head) {
        Node *tmp = head;
        head = head->next;
        free(tmp->data);
        free(tmp);
    }
}

int main() {
    Node *head = NULL;
    Node *tail = NULL;

    for (int i = 0; i < NODE_COUNT; ++i) {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "Node #%d", i);
        Node *new_node = create_node(i, buffer);

        if (!head)
            head = new_node;
        else
            tail->next = new_node;

        tail = new_node;
    }

    printf("Created %d nodes.\n", NODE_COUNT);

    // Uncomment the next line to observe the difference in memory leaks
    // free_list(head);

    return 0;
}

