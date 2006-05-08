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

/* standard task graph */
struct stg {
	int             procs;    /* Number of processors */
	int             tasks;    /* Number of tasks */  
	struct stg_task *task[];  /* Array of task entries */ 
};

/* per task */
struct stg_task {
	int             index;    /* Task index */
	int             ptime;    /* Computation time */
	int             preds;    /* Number of predecessors */
	struct stg_pred *pred[];  /* Array of predecessor entries */
};

/* per predecessor */
struct stg_pred {
	int index;  /* Predecessor index */
	int ctime;  /* Communication time */
};


/* read stg file and allocate task graph data structure */ 
extern struct stg *new_task_graph_from_file(char *fn);

/* print task graph on standard output */
extern void print_task_graph(struct stg *tg);

/* free task graph */
extern void free_task_graph(struct stg *tg);
