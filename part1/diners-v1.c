/*
 * Lucas Rappette
 *
 * Project 4: Dining Philosophers
 * Deadlocking Algorithm
 *
 */
#include "diners.h"

int main(int argc, char * argv[])
{

    int i;
    pthread_t * thread_id;
    srandom(time(NULL)); //Seed the random number generator only once.

    if( 0 == (process_command_line(argc, argv)))
    {
        printf("Time To Live (seconds)       :");
        print_padding(time_to_run, 4);
        printf("%d\n", time_to_run);
        printf("Number of Dining Philosophers:");
        print_padding(num_philosophers, 4);
        printf("%d\n", num_philosophers);
        printf("------------------------------\n");

        /* Allocate Necessary memory */
        thread_id = (pthread_t *) malloc(sizeof(pthread_t) * num_philosophers);
        state = (int *) malloc(sizeof(int) * num_philosophers);
        mutex = (semaphore_t *) malloc(sizeof(semaphore_t));
        write_lock = (semaphore_t *) malloc(sizeof(semaphore_t));
        chopsticks = (semaphore_t **) malloc(sizeof(semaphore_t *) * num_philosophers);
        phil_list = (philosopher_t **) malloc(sizeof(philosopher_t *) * num_philosophers);
        semaphore_create(mutex, 1);
        semaphore_create(write_lock, 1);
        for (i = 0; i < num_philosophers; i++)
        {
            chopsticks[i] = (semaphore_t *) malloc(sizeof(semaphore_t));
            semaphore_create(chopsticks[i], 1);

            phil_list[i] = (philosopher_t *) malloc(sizeof(philosopher_t));
            phil_list[i]->left = NULL;
            phil_list[i]->right = NULL;
            phil_list[i]->id = i;
            phil_list[i]->times_ate = 0;
            phil_list[i]->times_thought = 0;
        }

        for (i = 0; i < num_philosophers; i++)
        {
            pthread_create(&thread_id[i], NULL, philosopher, (void *) (intptr_t) &phil_list[i]->id);
        }

        for (i = 0; i < num_philosophers; i++)
        {
            pthread_join(thread_id[i], NULL);
        }
        /* End of thread simulation, I'm not even sure what else to call it */
        printf("------------------------------\n");

        for(i = 0; i < num_philosophers; ++i)
        {
            printf("Philosopher");
            print_padding(phil_list[i]->id, 4);
            printf("%d: Ate ", phil_list[i]->id);
            print_padding(phil_list[i]->times_ate, 3);
            printf("%d / Thought ", phil_list[i]->times_ate);
            print_padding(phil_list[i]->times_thought, 3);
            printf("%d\n", phil_list[i]->times_thought);

            free(phil_list[i]);

        }
        free(phil_list);
        printf("------------------------------\n");
        for (i = 0; i < num_philosophers; i++)
        {
            semaphore_destroy(chopsticks[i]);
            free(chopsticks[i]);
        }
        free(state);
        free(thread_id);
        free(chopsticks);
        semaphore_destroy(mutex);
        semaphore_destroy(write_lock);
        free(mutex);
        free(write_lock);

    }
    pthread_exit(NULL);
    return 0;
}

void* philosopher(void * phil_num)
{
    int * i = (int *) phil_num;
    int ret;

    think(*i);
    while(utime_to_run > 0)
    {
        state[*i] = HUNGRY;
        if( 0 != (ret = semaphore_wait(chopsticks[*i])) )
        {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, *i);
            pthread_exit(NULL);
        }
        if( 0 != (ret = semaphore_wait(chopsticks[(*i + 1) % num_philosophers])) )
        {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, *i);
            pthread_exit(NULL);
        }

        // eating
        eat(*i);

        //finishing
        if( 0 != (ret = semaphore_post(chopsticks[*i])) )
        {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, *i);
            pthread_exit(NULL);
        }
        if( 0 != (ret = semaphore_post(chopsticks[(*i + 1) % num_philosophers])) )
        {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, *i);
            pthread_exit(NULL);
        }
        state[*i] = DONE;

        if( 0 != (ret = semaphore_wait(write_lock)) )
        {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, *i);
            pthread_exit(NULL);
        }
        printf("Philosopher");
        print_padding(phil_list[*i]->id, 3);
        printf("%d: ", phil_list[*i]->id);
        printf("......\t....\tDone!\n");
        if( 0 != (ret = semaphore_post(write_lock)) )
        {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, *i);
            pthread_exit(NULL);
        }
        //thinking
        think(*i);
    }

    pthread_exit(NULL);
}
/*
*   Sets the required fields for the diner to run and checks for errors in args.
*/
int process_command_line(int argc, char ** argv)
{
    if(argc < 2)
    {
        printf("Error: The Time to run in seconds argument is required.\n");
        return -1;
    }
    else
    {
        time_to_run = atoi(argv[1]);
        if (time_to_run <= 0)
        {
            printf("Error: Time to run in seconds must be a positive integer greater than zero.\n");
            return -1;
        }
        utime_to_run = (long) ((long) time_to_run * (long) 1000000);
        if(argc > 2)
        {
            num_philosophers = atoi(argv[2]);
            if (num_philosophers <= 1)
            {
                printf("Error: Number of philosophers dining must be a positive integer greater than one.\n");
                return -1;
            }
        }
        else
        {
            num_philosophers = 5;
        }
    }

    return 0;
}

/*
* Not my utility function, but you can't make a better natural log algorithm.
* Originally used in one of my compilers assignment, this function computes
* natural log in an efficient manner.
*/
double ln(double x)
{
    double old_sum = 0.0;
    double xmlxpl = (x - 1) / (x + 1);
    double xmlxpl_2 = xmlxpl * xmlxpl;
    double denom = 1.0;
    double frac = xmlxpl;
    double term = frac;
    double sum = term;

    while ( sum != old_sum )
    {
        old_sum = sum;
        denom += 2.0;
        frac *= xmlxpl_2;
        sum += frac / denom;
    }
    return 2.0 * sum;
}
/*
*   Cheap trick to find the number of digits in a number. Surpisingly, log10 is
*   more accurate in calculation with a relatively less precise value of LN10.
*/
double log10(double x)
{
    if(x == 0)
    {
      return 0.;
    }

    return ln(x) / LN10;
}

/*
*   Method to automatically handle whitespace padding by finding the number of
*   digits in the number and to pad each digit to match formatting.
*   Drastically simplifies format matching.
*/
void print_padding(int i, int pad_length)
{
    /* Calculate padding for line formatting */
    int num_digits;
    int padding;
    num_digits = 1 + log10( (double) i);
    padding = pad_length - num_digits;
    while(padding > 0)
    {
        printf("%c", 32);
        --padding;
    }
}

void eat(int i)
{
    int ret;
    state[i] = EATING;
    long sl_time = (long) (random() % TIME_TO_SLEEP);
    usleep(sl_time);
    phil_list[i]->times_ate++;

    if( 0 != (ret = semaphore_wait(write_lock)) )
    {
        fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, i);
        pthread_exit(NULL);
    }
    printf("Philosopher");
    print_padding(phil_list[i]->id, 3);
    printf("%d: ", phil_list[i]->id);
    printf("......\tEat!\n");
    if( 0 != (ret = semaphore_post(write_lock)) )
    {
        fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, i);
        pthread_exit(NULL);
    }

    utime_to_run -= sl_time;
}

void think(int i)
{
    int ret;
    state[i] = THINKING;
    long sl_time = (long) (random() % TIME_TO_SLEEP);
    usleep(sl_time);
    phil_list[i]->times_thought++;

    if( 0 != (ret = semaphore_wait(write_lock)) )
    {
        fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, i);
        pthread_exit(NULL);
    }
    printf("Philosopher");
    print_padding(phil_list[i]->id, 3);
    printf("%d: ", phil_list[i]->id);
    printf("Think!\n");
    if( 0 != (ret = semaphore_post(write_lock)) )
    {
        fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, i);
        pthread_exit(NULL);
    }
    utime_to_run -= sl_time;
}
