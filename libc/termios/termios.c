#include <termios.h>

int tcgetattr(int fildes, struct termios *termios_p){
    ioctl(fildes, TCGETS, termios_p);
}

int tcsetattr(int fildes, int actions, struct termios *termios_p){
    ioctl(fildes, TCSETS, termios_p);
}