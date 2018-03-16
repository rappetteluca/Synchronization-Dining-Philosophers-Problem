/*
 * Samantha Foley
 *
 * Project 4: Dining Philosophers (Common header)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include "semaphore_support.h"


/*****************************
 * Defines
 *****************************/
#define TRUE  0
#define FALSE 1

/* 1.0 second = 1000000 usec */
//#define TIME_TO_SLEEP 100000
/* 0.5 second = 500000 usec */
//#define TIME_TO_SLEEP 50000
#define TIME_TO_SLEEP 5000

#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define DONE 3

#define LEFT(i, philo_count) ( (i + philo_count-1) % philo_count )
#define RIGHT(i, philo_count) ( (i + 1) % philo_count )

//Macro For Counting Digits. Implemented in compilers first.
//I chose base conversion to log10 from ln to count digits and avoid many
//lines of formatting.
//More precision hurts computation. All Floating Point Values suffer from this.
#define LN10 2.302585092994

/*****************************
 * Structures
 *****************************/

typedef struct philosopher {
  semaphore_t * left;
  semaphore_t * right;
  int id;
  int times_ate;
  int times_thought;
} philosopher_t;

/*****************************
 * Global Variables
 *****************************/

int * state; /* Initialize all states to 'THINKING' */
semaphore_t * mutex;
semaphore_t * write_lock;
semaphore_t ** chopsticks; /* Initialize all to '0' */
philosopher_t ** phil_list;
int num_philosophers;
int time_to_run;
long utime_to_run;

/*****************************
 * Function Declarations
 *****************************/

 void * philosopher(void * phil_num);
 void take_chopsticks(int i);
 void release_chopsticks(int i);
 void test(int i);
 int process_command_line(int argc, char ** argv);
 double ln(double x);
 double log10(double x);
 void print_padding(int i, int pad_length);
 void change_state(int phil_id, int new_state);
 void think(int i);
 void eat(int i);
