#define SYSCALL_WRITE 0
#define SYSCALL_READ 1
#define SYSCALL_MALLOC 2
#define SYSCALL_CALLOC 3
#define SYSCALL_FREE 4
#define SYSCALL_TIME 5
#define SYSCALL_PID 6
#define SYSCALL_UP 7
#define SYSCALL_DOWN 8
#define SYSCALL_SEND 9
#define SYSCALL_RECEIVE 10
#define SYSCALL_PROCESS 11


#define DESCRIPTOR_CLI 0
#define DESCRIPTOR_NET 1

int syscall(int a, int b, int c, int d);