#include <kernel/multitasking/task_state.h>
#include <kernel/multitasking/multitasking.h>
#include <string.h>

task_state* current_task = NULL;
task_state** pqueue = NULL;
unsigned int pqueue_pos = 0;
unsigned int pqueue_size = 0;
unsigned int max_pid = 0;
unsigned int max_tid = 0;

extern unsigned get_esp();

extern void switch_task_asm(task_state* next_task);

void switch_task(task_state* next_task){

    unsigned x = get_esp();
    printf("AHH- %X\n", x);

    x = next_task->esp;
    next_task->esp -= 4;

    *(next_task->esp) = next_task->pc;
    next_task->esp = x;
    switch_task_asm(next_task);
    printf("SWITCHED!\n");
    
}

void init_task(task_state* task, void* func){

    // task->pc = func;
    // *(task->esp) = task->pc;
    task->esp -= 1;

    unsigned int reg;
    __asm__("movl %%ebx, %0" : "=r"(reg));
    *(task->esp) = reg;
    task->esp -= 1;
    __asm__("movl %%esi, %0" : "=r"(reg));
    *(task->esp) = reg;
    task->esp -= 1;
    __asm__("movl %%edi, %0" : "=r"(reg));
    *(task->esp) = reg;
    task->esp -= 1;
    __asm__("movl %%ebp, %0" : "=r"(reg));
    *(task->esp) = reg;
    //task->esp -= 1;

    // *(task->esp) = task->pc;
    // task->esp -= 1;
        
    printf("STACK %X\n", task->esp);
}
extern unsigned get_esp();

void multitasking_init(){

    pqueue = malloc(sizeof(task_state) * MAX_PROCCESS_NUM);
}

void add_task(void* func){

    task_state* task = malloc(sizeof(task_state));
    task->addr_limit = ~0; // full address space
    task->pid = max_pid++;
    task->tid =  max_tid++;

    // figure out tty
    task->max_fds = current->max_fds;
    task->fdt = current->fdt;

    task->pc = func;

    pqueue[pqueue_size++] = task;
    //current_task = task;
    task->esp = malloc(25 * 100);
    task->esp += 25; // starts at the end of the stack

    init_task(task, func);

}