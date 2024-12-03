#include <stdio.h>
#include <pthread.h>

int shared_resource = 0;

void *increment_resource(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        shared_resource++; // Race condition
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, increment_resource, NULL);
    pthread_create(&t2, NULL, increment_resource, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final value: %d\n", shared_resource);

    return 0;
}

