/*
Filename: eight_time.h
Child files: eight_time.c

Contains useful utilities for measuring and using time */

#ifndef EIGHT_TIME_H
#define EIGHT_TIME_H

typedef void (*FuncType)(void *);
double clock_time(FuncType func, void *args);
double mono_time(FuncType func, void *args);

#endif
