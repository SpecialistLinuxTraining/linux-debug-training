#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main()
{
    int shmid;
    key_t key;
    char *shm;

    // Generate the same key used to create the shared memory segment
    key = ftok(".", 'R'); // Generates a unique key based on the file path ("." indicates the current directory) and a project identifier ('R').
    // By using a known file path, multiple programs can generate the same key and thus access the same shared memory segment,
    // provided they use the same path and project identifier.
    // is an arbitrary character used as a project identifier.
    // This helps distinguish different shared memory keys that might be generated from the same file path.
    if (key == -1)
    {
        perror("ftok");
        exit(1);
    }

    // Get the shared memory segment
    shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment in read-only mode. We could also attach in read/write mode (0), depending on our requirements
    shm = shmat(shmid, NULL, SHM_RDONLY);
    if (shm == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // Read and print the data from the shared memory segment
    printf("Data read from shared memory: %s\n", shm);

    // Detach the shared memory segment
    if (shmdt(shm) == -1)
    {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
