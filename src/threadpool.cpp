#include "threadpool.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>

/*
 * A threadpool must have the following parts:
 * threads:			a list of threads
 * jobs   :			jobs in thread pool
 * pool_lock:		pthread_mutex_lock for thread pool
 * job_ready:		pthread_cond_t for job ready
 * destroy:			flag indicate whether the pool shall be destroyed
 * size:			current size of jobs list
 * thread_num:		max thread num initialized in pool
 */
typedef struct thread_pool{
    pthread_mutex_t pool_lock;
    pthread_cond_t  job_ready;
    Job * jobs;
    int destroy;
    pthread_t * threads;
    unsigned int thread_num;
    int size;
}Thread_pool;
/* global Thread_pool variable*/
static Thread_pool * pool=NULL;

/*Initialize the thread pool*/
int pool_init(unsigned int thread_num)
{
    pool=(Thread_pool *)malloc(sizeof(Thread_pool));
    if(NULL==pool)
        return -1;
    pthread_mutex_init(&(pool->pool_lock),NULL);
    pthread_cond_init(&(pool->job_ready),NULL);
    pool->jobs=NULL;
    pool->thread_num=thread_num;
    pool->size=0;
    pool->destroy=0;
    pool->threads=(pthread_t *)malloc(thread_num * sizeof(pthread_t));
    
    int i;
    for(i=0;i<thread_num;i++){
        pthread_create(&(pool->threads[i]),NULL,routine,NULL);
    }
    return 0;
}
/*
 * Add job into the pool
 * assign it to some thread
 */
int pool_add_job(void *(*process)(void *),void *arg)
{
    Job * newjob=(Job *)malloc(sizeof(Job));
    newjob->process=process;
    newjob->arg=arg;
    newjob->next=NULL;
    pthread_mutex_lock(&(pool->pool_lock));
    Job * temp=pool->jobs;
    if(temp!=NULL){
        while(temp->next)
            temp=temp->next;
        temp->next=newjob;
    }
    else{
        pool->jobs=newjob;
    }
    
    /*For debug*/
    assert(pool->jobs!=NULL);
    pool->size++;
    pthread_mutex_unlock(&(pool->pool_lock));
    /*Rouse a thread to process this new job*/
    pthread_cond_signal(&(pool->job_ready));
    return 0;
}
/*Destroy the thread pool*/
int pool_destroy(void)
{
    if(pool->destroy)/*Alread destroyed!*/
        return -1;
    int i;
    pool->destroy=1;
    pthread_cond_broadcast(&(pool->job_ready));/*notify all threads*/
    for(i=0;i<pool->thread_num;i++)
        pthread_join(pool->threads[i],NULL);
    free(pool->threads);
    Job * head=NULL;
    while(pool->jobs){
        head=pool->jobs;
        pool->jobs=pool->jobs->next;
        free(head);
    }
    pthread_mutex_destroy(&(pool->pool_lock));
    pthread_cond_destroy(&(pool->job_ready));
    free(pool);
    pool=NULL;
    return 0;
}
/*Every thread call this function*/
void * routine(void *arg)
{
#ifdef DEBUG
    printf("start thread %u\n",pthread_self());
#endif
    while(1){
        pthread_mutex_lock(&(pool->pool_lock));
        while(pool->size==0 && !pool->destroy){
#ifdef DEBUG
            printf("thread %u is waiting\n",pthread_self());
#endif
            pthread_cond_wait(&(pool->job_ready),&(pool->pool_lock));
        }
        if(pool->destroy){
            pthread_mutex_unlock(&(pool->pool_lock));
#ifdef DEBUG
            printf("thread %u will exit\n",pthread_self());
#endif
            pthread_exit(NULL);
        }
#ifdef DEBUG
        printf("thread %u is starting to work\n",pthread_self());
#endif
        /*For debug*/
        assert(pool->size!=0);
        assert(pool->jobs!=NULL);
        pool->size--;
        Job * job=pool->jobs;
        pool->jobs=job->next;
        pthread_mutex_unlock(&(pool->pool_lock));
        /*process job*/
        (*(job->process))(job->arg);
        free(job);
        job=NULL;
    }
    /*You should never get here*/
    pthread_exit(NULL);
}




#ifdef TEST
void *test(void *arg)
{
    printf("thread %u is working on job %u\n",pthread_self(),*(int *)arg);
    sleep(1);
    
    return NULL;
}

int main(int argc,char **argv)
{
    int i;
    int *jobs=(int *)malloc(sizeof(int)*10);
    pool_init(3);
    for(i=0;i<10;i++){
        jobs[i]=i;
        pool_add_job(test,&jobs[i]);
    }
    sleep(5);
    pool_destroy();
    free(jobs);
    return EXIT_SUCCESS;
}
#endif