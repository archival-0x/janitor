#include "janitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
} test_t;


int main(void){
    test_t * t = (test_t *) jmalloc(sizeof(test_t));

    t->a = 4;
    t->b = 3;

    jfree(t);
}
