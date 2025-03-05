
.extern current_task
.extern tss_entry
# C declaration:
#   void switch_to_task(thread_control_block *next_thread);
#
# WARNING: Caller is expected to disable IRQs before calling, and enable IRQs again after function returns
.global switch_task_asm
switch_task_asm:
    push %ebx
    push %esi
    push %edi
    push %ebp


    mov current_task, %edi
    mov %esp, (%edi)

    # load next task
    movl 20(%esp), %esi
    mov %esi, current_task # change pointers
    mov (%esi), %esp # move real esp into esp
    mov 4(%esi), %eax
    mov 8(%esi), %ebx


    # mov tss_entry, %edi
    # mov %ebx, 4(%edi)

    # mov %cr3, %ecx


    # cmp %ecx, %eax
    # je doneVAS # check if address space is already equal
    # mov %eax, %cr3 # Load next task

    
doneVAS:
    pop %ebp
    pop %edi
    pop %esi
    pop %ebx


    ret


