#include "random_sleep.c"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int count = 0;
int thread_count = 0;
pthread_mutex_t lock;

void *thread_process_1(void *vptr, int num) {
  thread_count++;
  int sleeping = random_slee(pthread_self());
  printf("Thread %d beginning, thread %ld sleeping for %d seconds\n",
         thread_count, pthread_self(), sleeping);
  sleep(sleeping);
  printf("Thread %ld exiting\n", pthread_self());

  return NULL;
}

void *thread_process_2(void *vptr) {
  pthread_t child_thread;

  pthread_mutex_lock(&lock);

  if (count > thread_count) {
    thread_count++;

    int sleeping = random_slee(thread_count);

    fprintf(stderr, "Thread %d %ld making Thread %d ", thread_count,
            pthread_self(), thread_count + 1);
    pthread_create(&child_thread, NULL, thread_process_2, NULL);
    fprintf(stderr, "%ld\n", child_thread);

    sleep(sleeping);
    pthread_mutex_unlock(&lock);
  }

  return NULL;
}

void pattern1(int nums) {

  pid_t pid;
  pid_t pids[nums];
  for (int i = 1; i <= nums; i++) {
    pid = fork();

    if (pid == 0) {

      int sleeping = random_slee(i);
      printf("process %d beginning, pid %d sleeping for %d seconds\n", i,
             getpid(), sleeping);
      sleep(sleeping);
      printf("process %d exiting, pid %d\n", i, getpid());
      fflush(stdout);
      exit(0);
    } else if (pid > 0) {
      pids[i - 1] = pid;
    }
  }
  for (int i = 0; i < nums; i++) {
    waitpid(pids[i], NULL, 0);
  }
}

void pattern2(int nums) {
  // array to keep track of pids
  pid_t pids[nums];
  pids[0] = fork();

  for (int i = 0; i < nums; i++) {

    if (i < nums) {

      if (pids[i] == 0) {
        int sleeping = random_slee(i);
        pid_t p_id = getpid();
        printf("Process %d beginning, process: %d\n", i + 1, p_id);
        // child making next process
        pids[i + 1] = fork();

        if (pids[i + 1] == 0) {

          printf("Process %d %d making Process %d %d\n", i + 1, p_id, i + 2,
                 getpid());
          sleep(sleeping);
          printf("Process %d exiting\n", i + 1);
        } else if (pids[i + 1] > 0) {
          sleep(sleeping);
          waitpid(pids[i + 1], NULL, 0);
          break;
        }

      } else if (pids[i] > 0) {
        waitpid(pids[i], NULL, 0);
        break;
      }

    } else {
      break;
    }
  }
}

void pattern3(int nums) {
  pthread_t threads[nums];
  for (int i = 1; i <= nums; i++) {

    pthread_create(&threads[i], NULL, (void *)thread_process_1, NULL);
  }
  for (int i = 1; i <= nums; i++) {

    pthread_join(threads[i], NULL);
  }
}

void pattern4(int nums) {
  count = nums;
  pthread_t thread1;
  printf("Starting threads\n");
  pthread_mutex_init(&lock, NULL);

  pthread_create(&thread1, NULL, (void *)thread_process_2, NULL);
  while (thread_count < nums) {
    sleep(1);
  }
  pthread_join(thread1, NULL);
}