#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
	    if (!thread_param) {
		puts("threadfunc received thread_param pointer as NULL\n");
		return NULL;
	    }
    struct thread_data *thr_data = (struct thread_data *)thread_param;
    usleep(thr_data->obtain_ms);
    if (pthread_mutex_lock(thr_data->mutex) < 0) {	

		fprintf(stderr, "%s %ld %s %s", "Thread: ", *thr_data->thr, "failed pthread_mutex_lock with error:", strerror(errno));
		goto err;
    };
    usleep(thr_data->release_ms);
    if (pthread_mutex_unlock(thr_data->mutex) < 0) {
	    fprintf(stderr,"%s %ld %s %s","Thread: ", *thr_data->thr, "failed pthread_mutex_unlock with error:", strerror(errno));
	    goto err;
	   
    };
	    	
    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    thr_data->thread_complete_success = true;
    return (void *)thr_data; 
err:
	thr_data->thread_complete_success = false;
	return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{

	struct thread_data *thr_data = malloc(sizeof(struct thread_data));

		if (!thr_data) {
			fprintf(stderr, "%s\n", "malloc returned NULL");
			return false;
		}
	if (!mutex || !thread) {
		puts("Invalid arguments, thread or mutex NULL");
		goto err;
	}
	memset(thr_data, 0, sizeof(struct thread_data)); //Could use calloc but I did memset
	thr_data->thr = thread;
	thr_data->mutex = mutex;
	//We'll use usleep that takes in microseconds, 1 millisecond equals 1000 microseconds
	thr_data->obtain_ms = wait_to_obtain_ms * 1000; 
	thr_data->release_ms = wait_to_release_ms * 1000;
	if (pthread_create(thr_data->thr, NULL, threadfunc, (void *)thr_data) < 0) {
		perror("Failed to create thr");
		goto err;
	};

    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */

    return true;
    err:
    	free(thr_data);
	return false;

}

