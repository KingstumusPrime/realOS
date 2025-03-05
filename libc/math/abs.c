#include <math.h>

int abs(int x){
    if(x < 0){
        return x * -1;
    }
    return x;   
}

double fabs(double x){
    if(x < 0){
        return x * -1;
    }
    return x;   
}