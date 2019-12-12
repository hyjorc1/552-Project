#include "mapreduceconfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
using namespace std;

#define v 1 /* verbose flag, output if 1, \
                          no output if 0 */

typedef struct
{
    int id;       /* identification label */
    int nb;       /* number of jobs */
    int *nextjob; /* index of next job */
    int *work;    /* array of nb jobs */
} jobqueue;

jobqueue *make_jobqueue(int n);
/* 
 * Returns a queue of n jobs. */

void write_jobqueue(jobqueue *jobs);
/*
 * Writes the queue of jobs. */

int process_jobqueue(jobqueue *jobs, int n);
/*
 * Processes the queue of jobs with n threads,
 * returns the number of jobs done. */

void *do_job(void *args);
/*
 * Code to process one job. */

pthread_mutex_t read_lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{
    printf("How many jobs ? ");
    int njobs;
    scanf("%d", &njobs);
    jobqueue *jobs = make_jobqueue(njobs);
    if (v > 0)
        write_jobqueue(jobs);

    printf("How many threads ? ");
    int nthreads;
    scanf("%d", &nthreads);
    int done = process_jobqueue(jobs, nthreads);
    printf("done %d jobs\n", done);
    if (v > 0)
        write_jobqueue(jobs);

    return 0;
}

jobqueue *make_jobqueue(int n)
{
    jobqueue *jobs;

    jobs = (jobqueue *)calloc(1, sizeof(jobqueue));
    jobs->nb = n;
    jobs->nextjob = (int *)calloc(1, sizeof(int));
    *(jobs->nextjob) = 0;
    jobs->work = (int *)calloc(n, sizeof(int));

    int i;
    for (i = 0; i < n; i++)
        jobs->work[i] = 1 + rand() % 5;

    return jobs;
}

void write_jobqueue(jobqueue *jobs)
{
    int i;
    printf("%d jobs : ", jobs->nb);
    for (i = 0; i < jobs->nb; i++)
        printf(" %d", jobs->work[i]);
    printf("\n");
}

int process_jobqueue(jobqueue *jobs, int n)
{
    pthread_t t[n];
    pthread_attr_t a;
    jobqueue q[n];
    int i;

    printf("creating %d threads ...\n", n);
    for (i = 0; i < n; i++)
    {
        q[i].nb = jobs->nb;
        q[i].id = i;
        q[i].nextjob = jobs->nextjob;
        q[i].work = jobs->work;
        pthread_attr_init(&a);
        pthread_create(&t[i], &a, do_job, (void *)&q[i]);
    }
    printf("waiting for threads to return ...\n");
    for (i = 0; i < n; i++)
        pthread_join(t[i], NULL);

    return *(jobs->nextjob);
}

void *do_job(void *args)
{
    jobqueue *q = (jobqueue *)args;
    int dojob;
    do
    {
        dojob = -1;
        if (v > 0)
            printf("thread %d requests lock ...\n", q->id);
        pthread_mutex_lock(&read_lock);
        int *j = q->nextjob;
        if (*j < q->nb)
            dojob = (*j)++;
        if (v > 0)
            printf("thread %d releases lock\n", q->id);
        pthread_mutex_unlock(&read_lock);
        if (dojob == -1)
            break;
        if (v > 0)
            printf("*** thread %d does job %d ***\n", q->id, dojob);
        int w = q->work[dojob];
        if (v > 0)
            printf("thread %d sleeps %d seconds\n", q->id, w);
        q->work[dojob] = q->id; /* mark job with thread label */
        usleep(w);
    } while (dojob != -1);
    if (v > 0)
        printf("thread %d is finished\n", q->id);

    return NULL;
}