/*-
 * STG (Standard Task Graph) Format
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
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "stg.h"


/* read stg file and allocate task graph data structure */ 
struct stg*
new_task_graph_from_file(char *fn)
{
	FILE *fd;

	struct stg *tg;

	int line_number;
	char line[2048];
	char *result;
	int is_comment;

	int count;
	int tasks;
	int procs;

        int t;

	printf("reading '%s' .. ", fn);
	fd = fopen(fn, "r");
	if (fd == NULL) {
		printf("can't open '%s'!\n", fn);
		exit(EX_DATAERR);
	}

	tg = malloc(sizeof(struct stg));
	if (tg == NULL) {
		printf("can't allocate memory!\n");
		exit(EX_OSERR);
	}
	
	/* read first (non-comment) line */
	line_number = 0;
	do {
		line_number++;
		result = fgets(line, sizeof(line), fd);
		if (result == NULL) {
			if (feof(fd)) { 
				printf("unexpected end of file!\n");
				exit(EX_DATAERR);
			} else {
				printf("i/o error!\n");
				exit(EX_IOERR);
			}
		}
		/* a line starting with '#' is a comment */
		is_comment = ((strlen(result) > 0) && (result[0] == '#'));
	} while (is_comment);

	/* parse */
	count = sscanf(line, "%d %d", &tasks, &procs); 
	switch (count) {
	case EOF:
	case 0:
		printf("didn't find any number on line %d!\n", line_number);
		exit(EX_DATAERR);
	case 1:
		printf("found 'tasks' (%d), but not 'procs' on line %d!\n", 
		       tasks, line_number);
		exit(EX_DATAERR);
	case 2:
		tg->procs = procs;
		tg->tasks = tasks;
		break;
	default:
		printf("impossible error on line %d!\n", line_number);
	}

	/* read tasks */
	for (t = 0; t < tasks; t++) {
	
	}

	/* finish */
	fclose(fd);
	printf("done. (read %d lines)\n", line_number);
	return tg;
}


/* print task graph on standard output */
void
print_task_graph(struct stg *tg) 
{
	if (tg == NULL) {
		printf("can't print tg (invalid pointer)");
		return;
	}
	printf("procs: %d\n", tg->procs);
	printf("tasks: %d\n", tg->tasks);
	/* etc */
}


/* free task graph */
void
free_task_graph(struct stg *tg)
{
	/* free preds */
	/* free tasks */
	/* free head */
}
