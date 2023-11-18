#include <iostream>
#include <thread>

#include <pthread.h>
#include <unistd.h> // for getpid
#include <stdio.h>
#include <stdlib.h>
// #include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <vector>



int global_i_Cnt = 0;

void *p_function(void * data)
{
  pid_t pid; //process id
  pthread_t tid; // thread id

  pid = getpid(); //4
  tid = pthread_self();

  char* thread_name = (char *)data;
  

  while(global_i_Cnt<10)
  {
    printf("thread name : %s, tid : %x, pid : %u\n", thread_name, tid, (unsigned int)pid); //5
    global_i_Cnt++;
    
    // sleep(1);
  }

  int num1 = rand();
  // int num2 = 200;
  // int num3 = num1 + num2;
}

int main()
{
  pthread_t pthread[2];
  int thr_id;
  int status;
  char p1[] = "thread_1";
  char p2[] = "thread_222";
  char p3[] = "thread_33333";

  // sleep(1); //1

  thr_id = pthread_create(&pthread[0], NULL, p_function, (void*)p1); //2
  thr_id = pthread_create(&pthread[1], NULL, p_function, (void *)p2); //2
  thr_id = pthread_create(&pthread[2], NULL, p_function, (void *)p3); //2
  // p_function((void *)p3); //3

  pthread_join(pthread[0], (void **)&status); //6
  pthread_join(pthread[1], (void **)&status);
  pthread_join(pthread[2], (void **)&status);

  int num1 = rand();
  int num2 = 10;
  int num3 = num1 + num2;

  std::vector<int> tempvector;

  tempvector.push_back(12321);

  printf("endddddddd\n");

  return 0;
}