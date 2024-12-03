#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/types.h>

int exit_flag = 0; // Flag to signal threads to exit

void *threadFunction(void *arg)
{
    // Get and print the thread ID
    pid_t tid = syscall(__NR_gettid);
    printf("Thread ID: %d\n", tid);

    // Get and print the thread name
    char threadName[256];
    sprintf(threadName, "/proc/self/task/%d/comm", tid);
    FILE *threadNameFile = fopen(threadName, "r");
    fgets(threadName, sizeof(threadName), threadNameFile);
    fclose(threadNameFile);
    printf("Thread Name: %s", threadName);

    // Get and print the thread stack size
    struct rlimit stackLimit;
    getrlimit(RLIMIT_STACK, &stackLimit);
    printf("Thread Stack Size: %ld\n", stackLimit.rlim_cur);

    // Get and print the virtual memory mappings
    char mapsFilename[256];
    sprintf(mapsFilename, "/proc/self/task/%d/maps", tid);
    FILE *mapsFile = fopen(mapsFilename, "r");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), mapsFile) != NULL)
    {
        printf("Virtual Memory Mapping: %s", buffer);
    }
    fclose(mapsFile);

    // Get and print file descriptors
    char fdsDirectory[256];
    sprintf(fdsDirectory, "/proc/self/task/%d/fd", tid);
    DIR *fdsDir = opendir(fdsDirectory);
    struct dirent *entry;
    while ((entry = readdir(fdsDir)) != NULL)
    {
        if (entry->d_type == DT_LNK)
        {
            char fdPath[1024];
            sprintf(fdPath, "/proc/self/task/%d/fd/%s", tid, entry->d_name);
            char linkTarget[256];
            ssize_t linkSize = readlink(fdPath, linkTarget, sizeof(linkTarget) - 1);
            if (linkSize != -1)
            {
                linkTarget[linkSize] = '\0';
                printf("File Descriptor: %s -> %s\n", entry->d_name, linkTarget);
            }
        }
    }
    closedir(fdsDir);
    while (!exit_flag)
    {
        // Thread's work here...

        usleep(100000); // Sleep for 100ms (adjust as needed)
    }
    fprintf(stderr, "Exiting thread now ...\n");
    return NULL;
}

int main()
{
    int c;
    pthread_t thread1, thread2;
    pthread_attr_t attr1, attr2;

    // Get and print the process ID
    pid_t pid = getpid();
    printf("Process ID: %d\n", pid);

    // Get and print the process name
    char processName[256];
    sprintf(processName, "/proc/%d/comm", pid);
    FILE *processNameFile = fopen(processName, "r");
    fgets(processName, sizeof(processName), processNameFile);
    fclose(processNameFile);
    printf("Process Name: %s", processName);

    // Initialize attributes
    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);

    // Set different stack sizes
    size_t stackSize1 = 1024 * 1024;     // 1 MB
    size_t stackSize2 = 2 * 1024 * 1024; // 2 MB
    pthread_attr_setstacksize(&attr1, stackSize1);
    pthread_attr_setstacksize(&attr2, stackSize2);

    // Create threads with different stack sizes
    pthread_create(&thread1, &attr1, threadFunction, NULL);
    pthread_create(&thread2, &attr2, threadFunction, NULL);

    fprintf(stderr, "Press 'e' to exit...\n");

    while ((c = getchar()) != 'e')
    {
        // Keep looping until 'c' is pressed
    }
    // Set the exit flag to signal the threads to terminate
    exit_flag = 1;
    // Destroy attributes
    fprintf(stderr, "Destroying threads  ...\n");
    pthread_attr_destroy(&attr1);
    pthread_attr_destroy(&attr2);
    fprintf(stderr, "Joining threads  ...\n");
    // Wait for the threads to complete
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
