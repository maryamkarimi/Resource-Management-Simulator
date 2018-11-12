/******************************************************************************
* 
* Name: 	Zaid Albirawi
* Email: 	zalbiraw@uwo.ca
*
* simulate.h
*
******************************************************************************/

#ifndef SIMULATE_H
#define SIMULATE_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#include "job.h"
#include "linked_stack.h"
#include "helper.h"

#define SYSTEM_OUTPUT "system.out"
#define NUMBER_OF_THREADS 4


int memory, max_memory;
FILE *fp;
linked_stack_t *jobs;

void finish_job(job_t *);
void *do_job(job_t *);
void simulate(int, linked_stack_t*);

#endif