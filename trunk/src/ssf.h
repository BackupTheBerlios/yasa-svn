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

#if (!defined(_SSF_H_))
#define _SSF_H_

#include "stg.h"


/* 
 * Schedule, as used in the ssf output file.
 */

/* ssf + tasks * ssf_task */
struct ssf {
    int             tasks;   /* Number of tasks. (TPE: Target Processing Elements) */
    int             procs;   /* Number of processors. */
    struct ssf_task **task;  /* Array of task entries. */
};

struct ssf_task {
    int tindex;  /* Task index. */
    int proc;    /* Executing processor. */
    int start;   /* Start time. */
    int end;     /* End time. */
};


/* 
 * Status line (the last non-empty line of the .ssf file).
 * Line starts with a '#', values are separated by ';'.
 */
struct ssf_status {
    char          name[20];       /* Name of the used method, preferably with version. */
    double        ptime;          /* Computation time. (%.2f) */
    char          hostname[20];   /* Name of the host machine. */
    long long int ics;            /* Number of inspected complete schedules. (%lld) */
    long long int ips;            /* Number of inspected partial schedules. (%lld) */
    int           impsfirst;      /* Number of improvements of the first found solution. */
    int           slenfirst;      /* Schedule length of first found solution. */
    long long int treecuts;       /* Number of search tree cuts. (%lld) */
    double        avgcutheight;   /* Average cut height. (%.5f) */
    int           mincutheight;   /* Minimal cut height. */
    int           bestslen;       /* Best found schedule length. */
    double        minpsumnode;    /* Minimum computation time sum of a node. (%.2f) */  /* TODO ??? */
    double        avgpsumnode;    /* Average computation time sum of a node. (%.2f) */
    double        maxpsumnode;    /* Maximum computation time sum of a node. (%.2f) */
    long long int msgsbestfound;  /* Number of messages about best found solutions. (%lld) */
    int           slaves;         /* Number of slave machines. */
    long long int workpoolsize;   /* Size of workpool. (%lld) */  /* TODO ??? */
    double        psum;           /* Sum of all parallel computation times. (%.2f) */  /* TODO ??? */
};


/*
 * Internal schedule structure.
 *
 * For each processor we have a list of tasks.
 * We memorize the cost of the schedule, once it has been computed.
 */

struct iss {
    int             procs;   /* Number of processors. */
    struct iss_proc **proc;  /* procs * (Array of proc entries). */
    int             cost;    /* -1 if not calculated yet. */
};

struct iss_proc {
    int  tasks;  /* Number of tasks. */
    int  *task;  /* Array of task indices. */
};


extern struct ssf* new_schedule(struct stg *tg, int *malloced);
extern void print_schedule(struct ssf *schedule);
extern void write_schedule_to_file(char *fn, struct ssf *schedule, struct ssf_status *status);
extern void free_schedule(struct ssf *schedule, int *freed);

extern struct ssf_status* new_status(int *malloced);
extern void free_status(struct ssf_status *status, int *freed);

extern struct iss* new_iss(struct stg *tg, int *malloced);
extern void iss_add(struct iss *s, int p, int tindex, int *malloced, int* freed);
extern void print_iss(struct stg *tg, struct iss *s);

extern int cost(struct stg *tg, struct iss *s);

#endif
