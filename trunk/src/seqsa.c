/*-
 * Sequential Simulated Annealing Solver
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sa.h"
#include "tsort.h"

#include "seqsa.h"


/*
 * Sequential version.
 */
int
seqsa_solver(char *stg_filename, char *ssf_filename)
{
    int malloced;
    int freed;

    int malloced_initial;

    struct stg *tg;
    struct ssf_status *status;
    struct ssf *schedule;

    double eps;
    unsigned seed;

    int i;
    double t0;
    double t;

    struct iss *alpha;  /* present solution */
    struct iss *beta;   /* neighbour solution of alpha */

    double cost_alpha;
    double cost_beta;

    double r;
    double bf;  /* Boltzmann Factor p(alpha -> beta) */


    printf("** Sequential Simulated Annealing Solver\n");

    /* Allocate data structures. */
    malloced = 0;
    freed = 0;
    tg = new_task_graph_from_file(stg_filename, &malloced);
    status = new_status(&malloced);
    strncpy(status->name, "YASA Sequential", 20);

    eps = 1e-3;
    seed = 0;
    t0 = 1000.0;

    alpha = NULL;
    beta = NULL;

    /* Solve. */
    srand(seed);
    print_task_graph(tg);

    malloced_initial = 0;
    create_initial_solution(tg, alpha, &malloced_initial);
    printf("malloced %d bytes for initial solution\n", malloced_initial);
    cost_alpha = cost(tg, alpha);

    i = 0;
    t = t0;
    
    while (t > eps) {
        printf("i = %d, t = %f\n", i, t);
        select_neighbour(tg, alpha, beta);
        cost_beta = cost(tg, beta);
        if (cost_beta <= cost_alpha) {
            /* TODO alpha := beta */
            cost_alpha = cost_beta;
        } else {
            r = get_random();  /* r from (0, 1) */
            bf = boltzmann_factor(t, cost_alpha, cost_beta);
            printf("r = %f, bf = %f\n", r, bf);
            if (r < bf) {
                /* TODO alpha := beta */
                cost_alpha = cost_beta;
            }
        }
        i++;
        t = new_temp(t, i);
    }
    printf("stopping at i = %d, t = %f.\n", i, t);

    /* alpha to schedule */
    //schedule = new_schedule(tg, alpha, &malloced);
    schedule = new_schedule(tg, &malloced);
    printf("malloced %d bytes\n", malloced);

    /* Display Results */
    print_schedule(schedule);
    write_schedule_to_file(ssf_filename, schedule, status);

    /* Free data structures. */
    free_schedule(schedule, &freed);
    // TODO free initial solution
    free_status(status, &freed);
    free_task_graph(tg, &freed);
    printf("freed %d bytes => ", freed);
    if (malloced == freed)
        printf("OK.\n");
    else {
        printf("Error: malloced != freed!\n");
        return (1);
    }

    return (0);
}
