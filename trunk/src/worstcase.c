/*-
 * Worst Case Solver
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
#include <string.h>

#include "ssf.h"
#include "stg.h"
#include "worstcase.h"


/*
 * The worst case solver assigns all tasks to one processor.
 */
int
worstcase_solver(char *stg_filename, char *ssf_filename)
{
	int malloced;
	int freed;

	struct stg *tg;
	struct ssf_status *status;
	struct ssf *schedule;


	printf("** Worst Case Solver\n");

        /* Allocate data structures. */
	malloced = 0;
	freed = 0;
        tg = new_task_graph_from_file(stg_filename, &malloced);
	status = new_status(&malloced);
	strncpy(status->name, "YASA worstcase", 20);
	schedule = new_schedule(tg, &malloced);
	printf("malloced %d bytes\n", malloced);

	/* Solve. */
	print_task_graph(tg);
	printf("twiddle. twiddle, crunch, crunch..\n");
	print_schedule(schedule);
	write_schedule_to_file(ssf_filename, schedule, status);

	/* Free data structures. */
	free_schedule(schedule, &freed);
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
