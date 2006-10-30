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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "stg.h"


static void read_line_from_file(FILE *fd, char *line, int line_size, int *line_number, int ignore);
static void add_height(struct stg *tg); 
static int height(struct stg *tg, struct stg_task *ptask);
static struct stg_task* find_tindex(struct stg *tg, int tindex);


/*
 * Read stg file and allocate memory for the task graph data structure.
 */
struct stg *
new_task_graph_from_file(char *fn, int *malloced)
{
    FILE *fd;
    
    int msize;

    struct stg *tg;

    int line_number;
    char line[2048];

    int count;

    int tasks;
    int procs;

    int t;
    struct stg_task* ptask;
    int tindex;
    int ptime;
    int preds;

    int p;
    struct stg_pred* ppred;
    int ctime;

    assert(fn != NULL);
    assert(malloced != NULL);

    printf("reading '%s' .. ", fn);
    if ((fd = fopen(fn, "r")) == NULL) {
        printf("can't open '%s'!\n", fn);
        exit(EX_IOERR);
    }

    /* Allocate header structure. */
    msize = sizeof(struct stg);
    if ((tg = malloc(msize)) == NULL) {
        printf("can't allocate memory!\n");
        exit(EX_OSERR);
    }
    *malloced += msize;
    
    /* Read first (non-comment) line. */
    line_number = 0;
    read_line_from_file(fd, line, sizeof(line), &line_number, 1);

    /* Parse line for arguments. */
    count = sscanf(line, "%d %d", &tasks, &procs);
    switch (count) {
    case EOF:
    case 0:
        printf("didn't find any number on line %d!\n", line_number);
        exit(EX_DATAERR);
        /* NOTREACHED */
    case 1:
        printf("found 'tasks' (%d) but not 'procs' on line %d!\n",
               tasks, line_number);
        exit(EX_DATAERR);
        /* NOTREACHED */
    case 2:
        if (procs <= 0) {
            printf("'procs' (%d) is negative or zero on line %d!\n",
                   procs, line_number);
            exit(EX_DATAERR);
            /* NOTREACHED */
        }
        if (tasks < 0) {
            printf("'tasks' (%d) is negative on line %d!\n",
                   tasks, line_number);
            exit(EX_DATAERR);
            /* NOTREACHED */
        }
        tg->procs = procs;
        tg->tasks = tasks;
        break;
    default:
        printf("internal error while reading line %d!\n",
               line_number);
        exit(EX_SOFTWARE);
        /* NOTREACHED */
    }

    /* Allocate memory for task information. */
    msize = tasks * sizeof(struct stg_task*);
    if ((tg->task = malloc(msize)) == NULL) {
        printf("can't allocate memory!\n");
        exit(EX_OSERR);
    }
    *malloced += msize;

    /* Read tasks. */
    for (t = 0; t < tasks; t++) {
        /* Allocate. */
        msize = sizeof(struct stg_task);
        if ((ptask = malloc(msize)) == NULL) {
            printf("can't allocate memory!\n");
            exit(EX_OSERR);
        }
        *malloced += msize;
        tg->task[t] = ptask;

        /* Read task line. */
        read_line_from_file(fd, line, sizeof(line), &line_number, 1);

        /* Parse. */
        count = sscanf(line, "%d %d %d", &tindex, &ptime, &preds);
        switch (count) {
        case EOF:
        case 0:
            printf("didn't find any number on line %d!\n", line_number);
            exit(EX_DATAERR);
            /* NOTREACHED */
        case 1:
            printf("found 'tindex' (%d) but not 'ptime' and 'preds' on line %d!\n",
                   tindex, line_number);
            exit(EX_DATAERR);
            /* NOTREACHED */
        case 2:
            printf("found 'tindex' (%d) and 'ptime' (%d) but not 'preds' on line %d!\n",
                   tindex, ptime, line_number);
            exit(EX_DATAERR);
            /* NOTREACHED */
        case 3:
            /* Number of args is fine. */
            if (tindex != t) {
                printf("read 'tindex' (%d) but expected value %d on line %d!\n",
                       tindex, t, line_number);
                exit(EX_DATAERR);
                /* NOTREACHED */
            }
            if (ptime < 0) {
                printf("'ptime' (%d) is negative on line %d!\n",
                       ptime, line_number);
                exit(EX_DATAERR);
                /* NOTREACHED */
            }
            if (preds < 0 || preds >= tasks) {
                printf("'preds' (%d) is out of bounds on line %d!\n",
                       preds, line_number);
                exit(EX_DATAERR);
                /* NOTREACHED */
            }
            ptask->tindex = tindex;
            ptask->ptime = ptime;
            ptask->preds = preds;
            ptask->height = -1;
            break;
        default:
            printf("internal error while reading line %d!\n",
                   line_number);
            exit(EX_SOFTWARE);
            /* NOTREACHED */
        }
        
        /* Allocate memory for predecessor information. */
        msize = ptask->preds * sizeof(struct stg_pred*);
        if ((ptask->pred = malloc(msize)) == NULL) {
            printf("can't allocate memory!\n");
            exit(EX_OSERR);
        }
        *malloced += msize;
        
        /* Read preds. */
        for (p = 0; p < ptask->preds; p++) {
            /* Allocate. */
            msize = sizeof(struct stg_pred);
            if ((ppred = malloc(msize)) == NULL) {
                printf("can't allocate memory!\n");
                exit(EX_OSERR);
            }
            *malloced += msize;
            ptask->pred[p] = ppred;
            
            /* Read preds line. */
            read_line_from_file(fd, line, sizeof(line), &line_number, 1);
            
            /* Parse. */
            count = sscanf(line, "%d %d", &tindex, &ctime);
            
            switch (count) {
            case EOF:
            case 0:
                printf("didn't find any number on line %d!\n", line_number);
                exit(EX_DATAERR);
                /* NOTREACHED */
            case 1:
                printf("found 'tindex' (%d) but not 'ctime' on line %d!\n",
                       tindex, line_number);
                exit(EX_DATAERR);
                /* NOTREACHED */
            case 2:
                if (tindex < 0 || tindex >= tasks) {
                    printf("'tindex' (%d) is out of bounds on line %d!\n",
                           tindex, line_number);
                    exit(EX_DATAERR);
                    /* NOTREACHED */
                }
                if (ctime < 0) {
                    printf("'ctime' (%d) is negative on line %d!\n",
                           ctime, line_number);
                    exit(EX_DATAERR);
                    /* NOTREACHED */
                }
                ppred->tindex = tindex;
                ppred->ctime = ctime;
                break;
            default:
                printf("internal error while reading line %d!\n",
                       line_number);
                exit(EX_SOFTWARE);
                /* NOTREACHED */
            }
        }
    }

    /*
     * We expect the status line to be the first #-comment line behind the
     * task graph definitions.
     */
    read_line_from_file(fd, line, sizeof(line), &line_number, 0);

    /* Finish reading. */
    fclose(fd);
    printf("done. (read %d lines)\n", line_number);

    /* Was there a status line? */
    if (line) {
        printf("old status: [%s]\n", line);
        /* .. */
    }

    /* Calculate and add topological height. */
    add_height(tg);

    return (tg);
}


/*
 * Read a line from file 'fd' into buffer 'line', update line_number,
 * ignore '#'-comments if 'ignore' is not 0.
 */
static void
read_line_from_file(FILE *fd, char *line, int line_size, int *line_number, int ignore)
{
    char *s;
    int len;

    assert(fd != NULL);
    assert(line != NULL);
    assert(line_size > 0);
    assert(line_number != NULL);

    do {
        (*line_number)++;
        if (fgets(line, line_size, fd) == NULL) {
            if (feof(fd)) {
                printf("unexpected end of file!\n");
                exit(EX_DATAERR);
            } else {
                printf("i/o error!\n");
                exit(EX_IOERR);
            }
        }
        /* Ignore '#'-comment lines (if requested). */
        if (ignore && (s = strchr(line, '#')) != NULL)
            line[s - line] = '\0';  /* Discard comment. */
        len = strlen(line);
    } while (len == 0);
}


/* 
 * Add topological height. 
 */
static void 
add_height(struct stg *tg) 
{ 
    int tasks;
    int t;
    struct stg_task* ptask;

    tasks = tg->tasks;

    for (t = 0; t < tasks; t++) {
        ptask = tg->task[t];
        ptask->height = height(tg, ptask);
    }
}


/*
 * Calculate topological height of task *ptask.
 */
static int 
height(struct stg *tg, struct stg_task *ptask)
{
    int preds;
    int p;
    struct stg_pred *ppred;
    int tindex;
    struct stg_task *ptask2;
    int h;
    int h_max;

    preds = ptask->preds;
    if (preds == 0) {
        return 0;
    }

    h_max = -2;

    for (p = 0; p < preds; p++) {
        ppred = ptask->pred[p];
        tindex = ppred->tindex;
        ptask2 = find_tindex(tg, tindex);
        h = ptask2->height;
        if (h < 0) {
            h = height(tg, ptask2);
        }
        if (h > h_max) {
            h_max = h;
        }
    }
    return 1 + h_max;
}


static struct stg_task* 
find_tindex(struct stg *tg, int tindex) 
{
    int tasks;
    int t;
    struct stg_task *ptask;

    tasks = tg->tasks;

    for (t = 0; t < tasks; t++) {
        ptask = tg->task[t];
        if (tindex == ptask->tindex) {
            return ptask;
        }
    }
    return NULL;
}



/*
 * Return height for task tindex, if not found return -1.
 */
int 
get_height(struct stg *tg, int tindex)
{
    struct stg_task *ptask;
    
    ptask = find_tindex(tg, tindex);
    if (ptask == NULL) {
        return -1;
    }

    return ptask->height;
}


/*
 * Print task graph on standard output.
 */
void
print_task_graph(struct stg *tg)
{
    int tasks;
    int t;
    struct stg_task *ptask;

    int preds;
    int p;
    struct stg_pred *ppred;

    assert(tg != NULL);

    printf("tg = [\n");
    /* header */
    printf("  procs: %d\n", tg->procs);
    tasks = tg->tasks;
    printf("  tasks: %d\n", tasks);
    /* tasks */
    for (t = 0; t < tasks; t++) {
        ptask = tg->task[t];
        preds = ptask->preds;
        printf("  task %d: ptime = %d, preds = %d, height = %d\n",
               ptask->tindex, ptask->ptime, preds, ptask->height);
        /* predecessors */
        for (p = 0; p < preds; p++) {
            ppred = ptask->pred[p];
            printf("    pred %d: task = %d, ctime = %d\n",
                   p, ppred->tindex, ppred->ctime);
        }
    }
    printf("]\n");
}


/*
 * Free task graph.
 */
void
free_task_graph(struct stg *tg, int *freed)
{
    int tasks;
    int t;
    struct stg_task *ptask;

    int preds;
    int p;
    struct stg_pred *ppred;

    assert(tg != NULL);
    assert(freed != NULL);

    tasks = tg->tasks;
    for (t = 0; t < tasks; t++) {
        ptask = tg->task[t];
        preds = ptask->preds;
        for (p = 0; p < preds; p++) {
            /* Free pred info. */
            ppred = ptask->pred[p];
            *freed += sizeof(struct stg_pred);
            free(ppred);
        }
        /* Free array of pred info pointers. */
        *freed += preds * sizeof(struct stg_pred*);
        free(ptask->pred);

        /* Free task info. */
        *freed += sizeof(struct stg_task);
        free(ptask);
    }
    /* Free array of task info pointers. */
    *freed += tasks * sizeof(struct stg_task*);
    free(tg->task);

    /* Free tg header. */
    *freed += sizeof(struct stg);
    free(tg);

    /* Ensure that the given pointer points nowhere. */
    tg = NULL;
}
