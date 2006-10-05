/*-
 * Topological Sorting
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
#include <stdlib.h>

#include "stg.h"
#include "tsort.h"


/*
 * Check if for task graph 'tg' the task with index 'ti_a' is
 * allowed to execute before 'ti_b'.
 */

int
is_tg_less(struct stg *tg, int ti_a, int ti_b)
{
	int tasks;
	int t;
	struct stg_task *ptask;

	tasks = tg->tasks;
    //..	

	return 1;
}


/*
 * For a given task graph 'tg' (which defines the order relation)
 * read a list of task indices 'ti_list'.
 * Return a sorted list or an error.
 */


/*
 * Check if it is OK for task graph 'tg' to insert a task 'tindex_new' at
 * position 'pos' into the list of task indices 'tlist'.
 */

int
is_tinsert_ok(struct stg *tg, int tindex_new, int pos, int ti_list[])
{
	int tasks;
	int t;
	int tindex;

	assert(tg != NULL);
	tasks = tg->tasks;

	assert(0 <= tindex_new && tindex_new < tasks);
	assert(0 <= pos && pos <= tasks);

	for (t = 0; t < pos; t++) {
		tindex = ti_list[t];
		if (!is_tg_less(tg, tindex, tindex_new)) {
			return 0;
		}
	}
	for (t = pos; t < tasks; t++) {
		tindex = ti_list[t];
		if (!is_tg_less(tg, tindex_new, tindex)) {
			return 0;
		}
	}
        
	return 1;
}
