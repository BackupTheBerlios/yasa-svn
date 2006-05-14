/*-
 * YASA (Yet Another Simulated Annealing)
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

#include <stdio.h>

#include "worstcase.h"


/*
 * Yasa computes (near) optimal schedules for a given taskgraph.
 *
 * This is a NP hard problem. To deal with the large combinatorial
 * search space it uses the simulated annealing meta heuristics.
 *
 * It can make use of processor clusters through the pvm library.
 */
int
main(int argc, char* argv[])
{
	int solver;
	int rc;

	printf("** Hello, this is %s!\n", argv[0]);
	solver = 0;

	switch (solver) {
	case 0:
		rc = worstcase_solver("simple.stg", "simple-worstcase.ssf");
		break;
	default:
		printf("unknown solver %d!\n", solver);
		rc = 1;
	}
	return (rc);
}
