# CS441/541
## Project 4

## Author(s):

Lucas Rappette

## Date:

3/1/18


## Description:

A thread scheduling program that demonstrates the dining philosopher's Problem
and synchronization deadlock. The first version of the program presents deadlock as an issue,
the second version of the program fixes the deadlock problem by breaking the
hold and wait condition.

**Note:** *I apologize for the strange directories in this project, it was a two part assignment but this is always the classic synchronization problem*

## How to build the software

Simply type make on the command line in the proper directory (/part1/)  


## How to use the software

Execute the command './diners-v1' or './diners-v2' with at least one argument
on a UNIX-like shell.

Valid Arguments:
-The first arg is required, it sets the time in seconds for the main thread to be alive. (> 0)
-The second arg is optional and specifies the number of philosophers sitting down for
dinner that night. If no second arg is provided, the number of philosophers is
given a default value of 5. The minimum value is two for this arg.


## How the software was tested

Testing was completed by using outlier-like (i.e. num_philosophers = 100) input arguments in order to stress
test thread synchronization capabilities.


## Known bugs and problem areas

No known bugs at this time.

## Special Section

1. How can you determine when one or more philosophers are deadlocked in your application?

    If the application stops printing out new state changes while still alive then the system is in deadlock.

2. How can you determine when one or more philosophers are starving in your application?

    If a philosopher is starving in my application, he will have eaten very few times for how much he's thought;

3. Did deadlock occur every time you ran the deadlock-prone version of the solution?

    No, with usleep we get enough of an uncertain change in execution times between threads that the deadlock-prone version runs. However given enough time, the deadlock-prone version is certain to fail.

4. What happens when you change number of philosophers to 2? 4? 7? 100?

   Nothing unusual happens besides running v1 with a small number of philosophers increases the chances of deadlock.

5. While running v2 did you notice periodically a philosopher not consuming as much as the others?

    Not really, the average variance in resource consumption between the 5 processes was almost always within 5% of
    the average number of occurrences of resource consumption between all 5 processes. 
