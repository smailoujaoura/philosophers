#include <stdio.h>
#include <pthread.h>

#define RACE_CONDITION 100000
// 0 by default
int g_drinks_served;
void *serve_drink(void *arg)
{
	(void)arg;
 int i;
 i = 0;
 while (i++ < RACE_CONDITION)
 {
    //These a 3 machine instructions, LOAD-ADD-STORE
  //as you saw in assembly, our waiter is slow
  g_drinks_served++; 
  
  /*
  Scheduler can switch programs whenever it wants
  1)LOAD    <-
  2)ADD     <-
  3)STORE   <-
    The RACE_CONDITION value is proportional to the Probability of it happening
  Try to run the code with different values
  */
 }
 return (NULL);
}
int main(void)
{
 pthread_t waiter1;
 pthread_t waiter2;
 
 pthread_create(&waiter1, NULL, serve_drink, NULL);
 pthread_create(&waiter2, NULL, serve_drink, NULL);
 pthread_join(waiter1, NULL);
 pthread_join(waiter2, NULL);
 printf("Total drinks served: %d\\n", g_drinks_served);
 return (0);
}