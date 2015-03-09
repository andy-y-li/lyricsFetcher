/**
 *
*/





void * routine(void * arg);
int pool_add_job(void *(*process)(void * arg),void *arg);
int pool_init(unsigned int thread_num);
int pool_destroy(void);
//void *test(void *);
/* We define a queue of jobs which will be processed in thread pool*/
typedef struct job{
    void * (*process)(void *arg); /* process() will employed on job*/
    void *arg;/* argument to process */
    struct job * next;
}Job;

