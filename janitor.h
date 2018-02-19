#ifndef JANITOR_H
#define JANITOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {


} janitor_t;

void janitor_start(janitor_t *gc);
void janitor_stop(janitor_t *gc);
