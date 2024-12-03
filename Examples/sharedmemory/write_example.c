#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main()
{
    int shmid;        // Stores the shared memory ID.
    key_t key;        // Represents a key for identifying the shared memory segment.
    char *shm, *data; // Pointers used for accessing the shared memory

    // Generate a unique key
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

    // Create the shared memory segment
    // Creates a shared memory segment with the generated key, a size of SHM_SIZE,
    // and permission flags IPC_CREAT | 0666 (read and write permissions for all users).
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    // Attaches the shared memory segment identified by shmid to the process's address space.
    // It returns an address at which to attach the shared memory segment in the process's address space.
    shm = shmat(shmid, NULL, 0); // 0 for default behavior, but other flags like SHM_RDONLY (attach for reading only) can be used to make the segment read-only.
    if (shm == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // Write some data to the shared memory segment
    data = "Hello, shared memory!";
    sprintf(shm, "%s", data); // sprintf copies the data into the shared memory segment.

    printf("Data written to shared memory: %s\n", shm);

    // Detach the shared memory segment
    if (shmdt(shm) == -1)
    {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
