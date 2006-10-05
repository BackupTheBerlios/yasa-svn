/*-
 * SSF (Standard Schedule Format)
 *
 * Copyright (c) 2006 Marc van Woerkom <http://yasa.berlios.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "ssf.h"


/* 
 * Return a new schedule structure for a given task graph 'tg',
 * return malloced chars.
 */
struct ssf*
new_schedule(struct stg *tg, int *malloced)
{
	int msize;
	struct ssf *schedule;

	int tasks;
	int t;
	struct ssf_task *ptask;


	/* Allocate header structure. */
	msize = sizeof(struct ssf);
	if ((schedule = malloc(msize)) == NULL) {
		printf("can't allocate memory!\n");
		exit(EX_OSERR);
	}
	*malloced += msize;

	schedule->procs = tg->procs;
	tasks = tg->tasks;
	schedule->tasks = tasks;

	/* Allocate memory for task information. */
	msize = tasks * sizeof(struct ssf_task*);
	if ((schedule->task = malloc(msize)) == NULL) {
		printf("can't allocate memory!\n");
		exit(EX_OSERR);
	}
	*malloced += msize;

	for (t = 0; t < tasks; t++) {
		/* Allocate. */
		msize = sizeof(struct ssf_task);
		if ((ptask = malloc(msize)) == NULL) {
			printf("can't allocate memory!\n");
			exit(EX_OSERR);
		}
		*malloced += msize;
		schedule->task[t] = ptask;

		/* Initialize. */
		ptask->tindex = t;
		ptask->proc = -1;
		ptask->start = -1;
		ptask->end = -1;
	}

	return (schedule);
}


/*
 * Print schedule on standard output.
 */
void
print_schedule(struct ssf *schedule)
{
	int tasks;
	int t;
	struct ssf_task *ptask;

	if (schedule == NULL) {
		printf("Error: can't print schedule (invalid pointer)\n");
		return;
	}
	printf("schedule = [\n");
	/* header */
	printf("  procs: %d\n", schedule->procs);
	tasks = schedule->tasks;
	printf("  tasks: %d\n", tasks);
	/* tasks */
	for (t = 0; t < tasks; t++) {
		ptask = schedule->task[t];
		printf("  task %d: proc = %d, start = %d, end = %d\n",
		       ptask->tindex, ptask->proc, ptask->start, ptask->end);
	}
	/* TODO status */
	printf("]\n");
}


/*
 * Write schedule and its status to file 'fn'.
 */
void
write_schedule_to_file(char *fn, struct ssf *schedule, struct ssf_status *status)
{
	FILE *fd;
	int line_number;

	int tasks;
	int t;
	struct ssf_task *ptask;

	assert(fn != NULL);
	assert(schedule != NULL);
	assert(status != NULL);

	printf("writing '%s' .. ", fn);
	if ((fd = fopen(fn, "w")) == NULL) {
		printf("can't open '%s'!\n", fn);
		exit(EX_IOERR);
	}

	line_number = 0;

	/* Write schedule. */
	fprintf(fd, "%d %d\n", schedule->tasks, schedule->procs);
	line_number++;

	tasks = schedule->tasks;
	for (t = 0; t < tasks; t++) {
		ptask = schedule->task[t];
		fprintf(fd, "%d %d %d %d\n",
			ptask->tindex, ptask->proc, ptask->start, ptask->end);
		line_number++;
	}

	/* Write status line. */
	fprintf(fd, "#%s\n", status->name);
	line_number++;

	/* Finish writing. */
	fclose(fd);
	printf("done. (wrote %d lines)\n", line_number);
}


/*
 * Free schedule.
 */
void
free_schedule(struct ssf *schedule, int *freed)
{
	int tasks;
	int t;
	struct ssf_task *ptask;

	tasks = schedule->tasks;
	for (t = 0; t < tasks; t++) {
		ptask = schedule->task[t];
		/* Free task info. */
		*freed += sizeof(struct ssf_task);
		free(ptask);
	}
	/* Free array of task info pointers. */
	*freed += tasks * sizeof(struct ssf_task*);
	free(schedule->task);

	/* Free schedule header. */
	*freed += sizeof(struct ssf);
	free(schedule);

	/* Ensure that the given pointer points nowhere. */
	schedule = NULL;
}


/*
 * New status data structure.
 */
struct ssf_status*
new_status(int *malloced)
{
	int msize;
	struct ssf_status* status;

	/* Allocate. */
	msize = sizeof(struct ssf_status);
	if ((status = malloc(msize)) == NULL) {
		printf("can't allocate memory!\n");
		exit(EX_OSERR);
	}
	*malloced += msize;

	return (status);
}


/*
 * Free status data structure.
 */
void
free_status(struct ssf_status *status, int *freed)
{
	*freed += sizeof(struct ssf_status);
	free(status);
}


/* 
 * Create a new internal schedule struct, with empty task lists. 
 */
void new_iss(struct stg *tg, struct iss *s, int *malloced)
{
    int msize;
    int procs;
    int p;
    struct iss_proc *pproc;
    
    /* Allocate header structure. */
    msize = sizeof(struct iss);
    if ((s = malloc(msize)) == NULL) {
        printf("can't allocate memory!\n");
        exit(EX_OSERR);
    }
    *malloced += msize;
    
    procs = tg->procs;
    s->procs = procs;

    /* Allocate memory for processor list structures. */
    msize = procs * sizeof(struct iss_proc*);
    if ((s->proc = malloc(msize)) == NULL) {
        printf("can't allocate memory!\n");
        exit(EX_OSERR);
    }
    *malloced += msize;

    for (p = 0; p < procs; p++) {
        msize = sizeof(struct iss_proc);
		if ((pproc = malloc(msize)) == NULL) {
			printf("can't allocate memory!\n");
			exit(EX_OSERR);
		}
		*malloced += msize;
		s->proc[p] = pproc;

        pproc->proc = p;
        pproc->tasks = 0;
        pproc->task = NULL;
    }
    /* All processor lists are there now and empty. */
}


/*
 * Create initial solution.
 */

void 
create_initial_solution(struct stg *tg, struct iss *alpha, int *malloced)
{
    printf("creating initial solution\n");

    /* Create a new internal schedule struct, with empty task lists. */
    new_iss(tg, alpha, malloced);

    /* TODO implement algorithm Generate-Schedule from Hou/Ren. */
    
    /* GS1: Calculate height' for task graph. */
    /* GS2: Calculate sets G(height') of nodes. */
    /* GS3: For p-1 processors do GS4. */
    /* GS4: Form schedule for a processor. */
    /* GS5: Assign remaining nodes to last processor. */
}


/*
 * Calculate the cost of a given schedule s.
 */
double cost(struct stg *tg, struct iss *s)
{
    /* TODO */
    return 0.0;
}


/*
 * Pick a neigbour of alpha, return it in beta.
 *
 * THIS IMPLICITLY DEFINES THE NEIGHBOURHOOD CONCEPT OF THE PROBLEM!
 */
void
select_neighbour(struct stg *tg, struct iss *alpha, struct iss *beta)
{
    /* TODO */
    beta = NULL;
}


/*
 * Get random r, with 0 < r < 1.
 */
double get_random()
{
    double r;
    int ri;

    ri = rand();
    if ((ri == 0) || (ri == RAND_MAX)) {
        printf("get_random: got %d\n", ri);
        return get_random();  /* try again */
    }

    r = ((double) ri) / RAND_MAX;
    return r;
}


/*
 * Calculate Boltzmann Factor, i.e. the probability
 * of a transition from alpha to beta.
 */
double
boltzmann_factor(double t, double cost_alpha, double cost_beta)
{
    double bf;
    double dE;

    dE = cost_beta - cost_alpha;
    bf = exp(-dE / t);
    
    return bf;
}


/*
 * Temperature Reduction Function.
 */
double new_temp(double t, int i)
{
    double r;
    double t_new;

    r = 0.9;  /* typically 0.8 <= r <= 0.99 */
    t_new = r * t;

    return t_new;
}
