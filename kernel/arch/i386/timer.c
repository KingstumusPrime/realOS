#include <kernel/inline.h>
#include <kernel/irq.h>

// total ticks
int timer_ticks = 0;

// handler
void timer_handler(struct regs* reg0){
    timer_ticks++;

    if(timer_ticks%18 == 0){
        printf("One second passed\n");
    }
}

void timer_install(){
    irq_install_handler(0, timer_handler);
}