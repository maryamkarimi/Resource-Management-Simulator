/******************************************************************************
 * Name: Maryam Karimi Firozjaei
 * Email : mkarimif@uwo.ca
 * simulate.c is a resource management simulator that utilizes multi-thread
 * programming to execute multiple tasks simultaneously while keeping track of
 * memory resources.
 ******************************************************************************/

#include "simulate.h"

void simulate(int memory_value, linked_stack_t *stack) {

    /**************************************************************************
    * creates a file to write the output in it
    **************************************************************************/
    fp = fopen(SYSTEM_OUTPUT,"w");

    /**************************************************************************
    * Initializes jobs to the input stack, max_memory and memory to memory_value
    **************************************************************************/
    jobs = stack;
    max_memory = memory_value;
    memory = memory_value;

    /**************************************************************************
    * create NUMBER_OF_THREADS threads
    **************************************************************************/
    pthread_t threads[NUMBER_OF_THREADS];

    /**************************************************************************
    * Loops until the job stack is empty (i.e all jobs executed)
    **************************************************************************/
    while (jobs->size!=0){

        /**************************************************************************
        * loop_num keeps track of number of jobs being popped at a time
        * We need this number to know how many threads we are creating/ joining
        * it is equal to NUMBER_OF_THREADS if we have enough jobs for every thread
        **************************************************************************/
        int loop_num;

        if (jobs->size > NUMBER_OF_THREADS){
            loop_num = NUMBER_OF_THREADS;
        }

        else{
            loop_num = jobs->size;
        }

        /**************************************************************************
        * create an array of job_t that will hold the jobs that will be popped
        **************************************************************************/
        job_t *cur_jobs [loop_num];

        /********************************************************************************
        * create an array of bool that shows if thread[i] is created
        * all of them are set to false initially, and changed to true upon thread creation
        * we need these to know if a thread needs to be joined or not.
        *********************************************************************************/
        bool is_thread_created[NUMBER_OF_THREADS];

        for (int i=0; i<NUMBER_OF_THREADS; i++){
            is_thread_created[i] = false;
        }


        /**************************************************************************
        * Loops loop_num times to pop the jobs to be executed with threads now
        **************************************************************************/
        for (int i=0; i<loop_num;i++){

            job_t *cur_job = pop(stack);
            cur_jobs[i] = cur_job;

            /*****************************************************************************************
            * if the job's required memory is greater than max_memory, print_exceed_memory from helper
            *****************************************************************************************/
            if (cur_job->required_memory > max_memory){
                print_exceed_memory(fp,cur_job->number);
            }

            /*********************************************************************************************
             * if the job's required memory is less than max_memory:
             * 1. print_starting from helper
             * 2. create a thread for this job (i.e call do_job(cur_job))
             * 3. set is_thread_created[i] to true so we know threads[i] was created and needs to be joined
             *********************************************************************************************/
            else {
                print_starting(fp,cur_job->number);
                pthread_create(&threads[i], NULL,(void *) &do_job,(void *) cur_job);
                is_thread_created[i] = true;
            }
        }


        /*********************************************************************************************
         * after creating the threads, we use pthread_join on all of the threads that were created
         * to make sure do_job is terminated (i.e completed)
         *********************************************************************************************/
        for (int i=0; i<loop_num;i++) {
            if (is_thread_created[i]){
                pthread_join(threads[i], NULL);
            }
        }
    }

    /***************************************************************************
    * close the output file
    ****************************************************************************/
    fclose(fp);
}


/*********************************************************************************
* do_job is the function specified by threads. this function is used by simulate
*********************************************************************************/
void *do_job(job_t *job){

    /****************************************************************
    * if we have sufficient memory for this job to be executed,
    * print_allocate_memory, sleep for job's required time so the job
    * is done executing, and then call finish_job(job)
    ****************************************************************/

    memory-=job->required_memory;

    if (memory > 0){
        print_allocate_memory(fp,memory,job->required_memory);
        sleep(job->required_time);
        finish_job(job);
    }

    /******************************************************************************
    * if we don't have sufficient memory for this job to be executed,
    * deallocate the memory since this job's not being executed at this time,
    * call print_insufficient_memory from helper and push the job back to the stack
    ******************************************************************************/
    else{
        memory+=job->required_memory;
        print_insufficient_memory(fp,job->number);
        push(jobs,(void *) job);
    }

}

/**************************************************************************
* this function finishes a job by de-allocating memory,
* calling print_deallocate_memory and print_completed
**************************************************************************/
void finish_job(job_t *job){
    memory+=job->required_memory;
    print_deallocate_memory(fp,memory,job->required_memory);
    print_completed(fp,job->number);
}