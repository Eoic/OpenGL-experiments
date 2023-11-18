#include "object.h"
#include <stdio.h>

void printSomething() {
    printf("Hello. \n");
}

void init_programObject(programObject_t* o) {
    o->value = 42;
    o->pFnMemberFunc = printSomething;
}