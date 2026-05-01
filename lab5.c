// lab5.c
// Partner Names: Miko Lemecha, Artyom Kesler
// Due April 29, 2026
// For 'Lab 5: Virtualization'


#include <stdio.h>    // for printf()
#include <stdlib.h>   // for exit()
#include <unistd.h>   // for fork(), getpid(), and sleep().
#include <sys/types.h>// defines pid_t (Process ID type)
#include <sys/wait.h> // for wait()
#include <sys/mman.h> // for mmap()
#include <signal.h>   // for kill() and signal()
#include <string.h>   // for formatting text

// Pointer holds all PIDs; mmap will ensure all VMs can see this array when needed.
pid_t *vm_pids;

void handle_signal(int sig) {
    printf("[VM2] Signal received from VM1.\n");
}

void vm_behavior(int vm_id) {
    char my_dir[32], other_dir[32]; // Hold the file paths in these arrays.

    // Google Gemini saved a dozen lines of code for Part 2 by suggesting the below string formatting tricks:

    // If vm_id is 1, my_dir becomes "/mnt/vm1/test.txt"
    sprintf(my_dir, "/mnt/vm%d/test.txt", vm_id);

    // Math trick. For VM1, look at VM2. For VM2, look at VM3. For VM3, look at VM1.
    sprintf(other_dir, "/mnt/vm%d/test.txt", (vm_id % 3) + 1);

    printf("[VM%d] has a Process ID of %d\n", vm_id, getpid());

    // If this specific process is VM2, tell it to listen for 'SIGUSR1' (a custom user signal).
    // By default, if an OS sends a signal to a process, the process instantly dies.
    // This upcoming line handles the signal before the killing occurs (Part 4).
    if (vm_id == 2) {
        signal(SIGUSR1, handle_signal);
    }

    // Try to open a file in the directory created earlier. 'w+' is 'write mode, and also give me read permissions'.
    FILE *fp = fopen(my_dir, "w+");
    if (fp) { // If it opened successfully
        fprintf(fp, "Successfully entered VM%d; can you read me?\n", vm_id);
        rewind(fp); // Move cursor back to beginning of the line that was just printed right above this line.
        char buffer[64];
        fgets(buffer, sizeof(buffer), fp);
        printf("[VM%d] Data logged: %s", vm_id, buffer);
        fclose(fp);
    } else {
        printf("[VM%d] Error: Cannot open %s.\n", vm_id, my_dir);
    }

    // Isolation check (Part 3)
    printf("[VM%d] Testing for file system isolation: attempting to read %s\n", vm_id, other_dir);
    FILE *fp2 = fopen(other_dir, "r"); // "r" means read-only mode
    if (fp2) {
        // VMs are not properly isolated.
        printf("[VM%d] Access granted to %s!\n", vm_id, other_dir);
        fclose(fp2);
    } else {
        // VMs are isolated enough that one cannot read the files of another.
        printf("[VM%d] Access denied due to VM isolation.\n", vm_id);
    }

    // Terminal has been known to become a jumbled mess unless we wait 2s.
    sleep(2);

    if (vm_id == 1) {
        printf("VM1 is now sending SIGUSR1 to VM2 (PID: %d)...\n", vm_pids[1]);
        kill(vm_pids[1], SIGUSR1); // kill() should just be called send_mail(); here, it sends the SIGUSR1 message.
    }

    printf("While [VM%d] sleeps for 30s, kill it with 'kill -9 %d'\n", vm_id, getpid());
    for(int i=0; i<30; i++) {
        sleep(1);
    }

    printf("[VM%d] survived the timer. Shutting down gracefully.\n", vm_id);
    exit(0);
}

int main() {
    // mmap() is a black box function letting me designate some RAM that all cloned processes can share.
    // PROT_READ | PROT_WRITE means they can read and write to it. MAP_SHARED makes it visible to all processes.
    // idk what MAP_ANONYMOUS is but research shows that it goes there.
    vm_pids = mmap(NULL, 3 * sizeof(pid_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    printf("Creating 3 virtual machines...\n");

    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // pid == 0 proves we are the child process (the clone).
            sleep(1); // Give the parent a split second to save child's PID to the shared memory (just in case).
            vm_behavior(i + 1); // VM logic (passing 1, 2, or 3 as the ID)
        } else {
            // fork() returns the child's actual PID to its parent process.
            // The parent saves the child's ID into the shared memory array so the others can see it!
            vm_pids[i] = pid;
        }
    }

    // The Parent reaches here and will now wait() for all 3 children to finish to prevent zombie outbreak.
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    // Giving memory back to OS after using mmap()
    munmap(vm_pids, 3 * sizeof(pid_t));

    printf("All processes are complete.\n");
    return 0;
}
