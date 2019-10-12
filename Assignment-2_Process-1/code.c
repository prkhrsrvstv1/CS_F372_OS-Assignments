#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#define print_process printf("PID : %d\tPPID : %d\n",getpid(),getppid())

typedef struct {
    pid_t queue[1000];
    int front, rear;
    int size, level, n_children;
} Node;

void node_init(Node *node, int level, int self_pid);
void node_print_queue(Node *node);
int main(int argc, char *argv[]) {
    // Parse the command-line arguments and store them
    int a = atoi(argv[1]), b = atoi(argv[2]);
    pid_t pid;
    // Create and attach the shared parent data structure
    int shm_id = shmget(IPC_PRIVATE, sizeof(Node), 0666|IPC_CREAT);
    Node *node = (Node *)shmat(shm_id, NULL, 0);
    // Initialize the parent node at level b (top: b, leaves: 0)
    node_init(node, b, getpid());
    print_process;
    node_print_queue(node);

    // Start building the process tree
    for(int i = 0; i < a; ++i) {
        // Create a new process
        pid = fork();
        // If error
        if(pid < 0) {
            printf("Process creation failed. Exiting.\n");
            while(wait(NULL) != -1);
            exit(1);
        }
        // For the child process
        if(!pid) {
            // TODO
            
        }
        // For the parent process
        else {
            // TODO
        }

    }

    while(wait(NULL) != -1);
    return 0;
}

void node_init(Node *node, int level, int self_pid) {
    node->front = 0;
    node->rear = 2;
    node->level = level;
    node->queue[0] = self_pid;
    node->queue[1] = 0;
    node->n_children = 0;
    node->size = 2;
}

void node_print_queue(Node *node) {
    for(int i = node->front; i < node->rear; ++i)
        printf("%d ", node->queue[i]);
    printf("\n");
 }