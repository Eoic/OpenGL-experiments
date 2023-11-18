#ifndef OBJECT_H
#define OBJECT_H

typedef struct {
    int value;
    void (*pFnMemberFunc)(void);
} programObject_t;

void init_programObject(programObject_t*);

#endif