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
