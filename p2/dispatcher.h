#ifndef DISPATCHER_H
#define DISPATCHER_H

void initThreads();

void yield();

int startThread(void (*func)(void));

#endif