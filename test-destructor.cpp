#include <stdio.h>
#include <stdlib.h>

struct something {
    int x, y;
    char *greeting;
    something(int X, int Y) : x(X), y(Y) {
        greeting = (char *)malloc(5);
        greeting[0] = 'h';
        greeting[1] = 'h';
        greeting[2] = 'h';
        greeting[3] = 'h';
        greeting[4] = 'h';

    };
    ~something(){
        free(greeting);
        printf("i am destructing!\n");
    };
};

void doSomething(something whatever){
    printf("whatevs.x: %d\n", whatever.x);
}

int main(){
    doSomething(something(1,2));
}