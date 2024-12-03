#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
/*

The message "sched_setscheduler: Operation not permitted" means that the current process does not have the required permissions to set its scheduling policy or priority.

In Linux, only processes with superuser privileges or processes with the CAP_SYS_NICE capability can modify their scheduling policies and priorities. If a non-privileged process tries to set a scheduling policy or priority that requires elevated privileges, the sched_setscheduler call will fail with the "Operation not permitted" error.

To resolve this issue, you can either run your program with superuser privileges (using sudo or a similar tool), or you can grant your process the CAP_SYS_NICE capability using the setcap command.

sudo setcap CAP_SYS_NICE=+ep /path/to/your/program


SCHED_OTHER (Completely Fair Scheduler - CFS):
Default scheduling policy for most user-level tasks.
Utilizes a time-sharing mechanism with dynamic priority adjustment.
Fairly allocates CPU time among tasks to provide equal opportunity for execution.
Suitable for general-purpose tasks and ensures responsiveness.

SCHED_RR (Round Robin):
A real-time scheduling policy that provides each task with a fixed time slice (quantum) to execute.
Tasks are scheduled in a round-robin fashion, allowing each task to run for its quantum or until it blocks or yields the CPU.
Useful for tasks that require time-sliced execution in real-time systems.

SCHED_BATCH:
A scheduling policy designed for background batch processing tasks.
Typically used for tasks with lower priority that shouldn't impact interactive or real-time tasks.
Allows tasks to use more significant time slices to maximize CPU utilization.

SCHED_IDLE:
Reserved for the Linux kernel's idle task, which runs when no other tasks are available to execute.
Runs at the lowest priority and consumes minimal CPU resources.

SCHED_DEADLINE:
A real-time scheduling policy designed for tasks with hard deadlines.
Allows tasks to specify their execution time requirements and deadlines.
Ensures that tasks with higher deadlines get CPU time priority.
Suitable for applications with strict timing constraints.

SCHED_ISO (SCHED_ISO_FULL, SCHED_ISO_LOAD):
Scheduling policies for isolating real-time tasks from non-real-time tasks.
Used to create isolated execution environments for critical real-time tasks.
Helps prevent interference from non-real-time tasks.

SCHED_BATCH_IM (SCHED_BATCH_IDLEMATIC):
A scheduling policy designed for background batch processing tasks with improved interactive responsiveness.
Provides a balance between batch processing and interactivity.
Suitable for tasks that need to run in the background without significantly affecting interactive tasks.
These are some of the commonly used scheduling policies in the Linux kernel. The choice of scheduling policy depends on the specific requirements of the tasks and applications being run on a Linux system. Developers and system administrators should select the appropriate policy to achieve the desired system behavior and responsiveness.

*/

int main()
{
    struct sched_param sp;
    char c;
    int policy = SCHED_FIFO; // First-in, First-out scheduling policy SCHED_FIFO is typically used in real-time systems and applications where meeting strict deadlines and deterministic behavior is crucial. Examples include industrial automation, robotics, multimedia processing, and embedded systems.
    sp.sched_priority = 99;  // Set priority to 99 (highest)

    if (sched_setscheduler(0, policy, &sp) == -1)
    {
        perror("sched_setscheduler");
        exit(1);
    }
    printf("Type 'e' and press enter to exit\n");
    while ((c = getchar()) != 'e')
    {
        // Keep looping until 'c' is pressed
        // Yield the CPU to allow other threads to run
        sched_yield();
    }

    return 0;
}
