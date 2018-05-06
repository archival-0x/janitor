#include "janitor.h"

struct test {
  int a;
  int b;
};

typedef struct test test_t;

int main(void){
  test_t * t = (test_t *) jmalloc(sizeof(test_t));

  t->a = 4;
  t->b = 3;

  jfree(t);
}
