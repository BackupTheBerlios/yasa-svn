/*-
 * SA (Simulated Annealing)
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

#include "sa.h"


/*
 * Create initial solution.
 */

struct iss* 
create_initial_solution(struct stg *tg, int *malloced, int *freed)
{
    struct iss *alpha;

    int p;
    int tasks;
    int t;
    struct stg_task *task;
    int tindex;

    printf("creating initial solution\n");

    /* Create a new internal schedule struct, with empty task lists. */
    alpha = new_iss(tg, malloced);

    /* TODO implement algorithm Generate-Schedule from Hou/Ren. */
    
    /* GS1: Calculate height' for task graph. */
    /* GS2: Calculate sets G(height') of nodes. */
    /* GS3: For p-1 processors do GS4. */
    /* GS4: Form schedule for a processor. */
    /* GS5: Assign remaining nodes to last processor. */


    /* Quick hack */
    p = 0;
    tasks = tg->tasks;
    for (t = 0; t < tasks; t++) {
        task = tg->task[t];
        tindex = task->tindex;
        iss_add(alpha, p, tindex, malloced, freed);
    }

    print_iss(tg, alpha);

    return alpha;
}


/*
 * Pick a neigbour of alpha, return it in beta.
 *
 * THIS IMPLICITLY DEFINES THE NEIGHBOURHOOD CONCEPT OF THE PROBLEM!
 */
struct iss*
select_neighbour(struct stg *tg, struct iss *alpha, int *malloced, int *freed)
{
    struct iss *beta;

    int procs;

    int p1;
    struct iss_proc *pproc1;
    int tasks1;
    int t1;

    int p2;
    struct iss_proc *pproc2;
    int tasks2;
    int t2;

    int tasks_new;
    int *task_new;
    int msize;
    int t;

    int ok;
    int h1;
    int h2;
    
    int p;
    struct iss_proc *pproc;

    int i;
    i = 0;


    procs = tg->procs;

    while (1) {
        beta = new_iss(tg, malloced);

        /* Source: Pick a random (p1, t1) from alpha. */
        while (1) {
            p1 = get_random(procs);
            pproc1 = alpha->proc[p1];
            tasks1 = pproc1->tasks;
            if (tasks1 > 0) {
                t1 = get_random(tasks1);
                break;
            }
        }

        /* Target: Choose a random (different) new position (p2, t2). */
        while (1) {
            p2 = get_random(procs);
            pproc2 = alpha->proc[p2];
            tasks2 = pproc2->tasks;
            if (p2 == p1) {
                /* Move source within its processor list. */
                t2 = get_random(tasks2 + 1);  /* +1! */
            } else {
                /* Move between different lists. */
                if (tasks2 == 0) {
                    t2 = 0;
                } else {
                    t2 = get_random(tasks2);
                }
            }
            /* Different? */
            if ((p2 != p1) || (t2 != t1)) {
                break;
            }
        }
        printf("try move (%d, %d) -> (%d, %d)\n", p1, t1, p2, t2);

        /* Will this result in a valid schedule? */
        
        if (p2 == p1) {
            tasks_new = tasks2;
        } else {
            tasks_new = tasks2 + 1;
        }
        msize = tasks_new * sizeof(int);
        if ((task_new = malloc(msize)) == NULL) {
            printf("can't allocate memory!\n");
            exit(EX_OSERR);
        }
        *malloced += msize;
        if (p2 == p1) {
            /* A permutation on the list. */
            for (t = 0; t < tasks2; t++) {
                if (t == t2) {
                    task_new[t] = pproc1->task[t1];
                } else if (t == t1) {
                    task_new[t] = pproc1->task[t2];
                } else {
                    task_new[t] = pproc1->task[t];
                }
            }
        } else {
            /* An insertion on the new list. */
            for (t = 0; t < t2; t++) {
                task_new[t] = pproc2->task[t];
            }
            task_new[t2] = pproc1->task[t1];
            for (t = t2; t < tasks2; t++) {
                task_new[t + 1] = pproc2->task[t];
            }
        }

        /* Just check the target processor list. */
        ok = 1;
        h1 = get_height(tg, task_new[0]);
        printf("check h: %d", h1);
        for (t = 1; t < tasks_new; t++) {
            h2 = get_height(tg, task_new[t]);
            printf(" %d", h2);
            if (h2 < h1) {
                /* Invalid. */
                ok = 0;
                break;
            }
            h1 = h2;
        }
        printf(" ok = %d\n", ok);

        if (ok == 1) {
            /* Create beta structure. */
            /* TODO Free old plist? */
            beta->procs = procs;
            if (p2 != p1) {
                for (p = 0; p < procs; p++) {
                    pproc = beta->proc[p];
                    if (p == p1) {
                        /* TODO Don't forget to remove the source element in case p2 != p1. */
                        /* put the pruned list in tasks_new2, task_new2 */
                        /*
                        pproc->tasks = tasks_new2;
                        pproc->task = task_new2;
                        */
                    } else if (p == p2) {
                        pproc->tasks = tasks_new;
                        pproc->task = task_new;
                    } else {
                        pproc->tasks = alpha->proc[p]->tasks;
                        pproc->task = alpha->proc[p]->task;  /* TODO copy over. */
                    }
                }
            } else {
                for (p = 0; p < procs; p++) {
                    if (p == p1) {
                    } else if (p == p2) {
                    } else {
                    }
                }
            }

            /* Leave infinte loop. */
            break;
        }
    }

    return beta;
}


/*
 * Get random double r, with 0 < r < 1.
 */
double 
get_random_r()
{
    double r;
    int ri;

    ri = rand();
    if ((ri == 0) || (ri == RAND_MAX)) {
        printf("get_random: got %d\n", ri);
        return get_random_r();  /* try again */
    }

    r = ((double) ri) / RAND_MAX;
    return r;
}


/*
 * Get random integer i, with 0 <= i < max.
 */
int
get_random(int max)
{
    int ri;
    int i;

    ri = rand();
    
    i = lround(((double) ri / RAND_MAX) * (max - 1));
    return i;
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
double 
new_temp(double t, int i)
{
    double r;
    double t_new;

    r = 0.9;  /* typically 0.8 <= r <= 0.99 */
    t_new = r * t;

    return t_new;
}
