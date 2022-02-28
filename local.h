#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define NUM_OF_PLAYERES 4

typedef struct {
  int playerNumber;
} ThreadParam;

pthread_mutex_t ballLock = PTHREAD_MUTEX_INITIALIZER;
pthread_t players[NUM_OF_PLAYERES];

int playerErrors[NUM_OF_PLAYERES] = {0, 0, 0, 0};
bool ball[NUM_OF_PLAYERES] = {1, 0, 0, 0};
bool isFinished = 0;

void exitProgram();
void printOnScreen(int from, int to, int goodOrBad);
void *playerRoung(void *arg);

void exitProgram() {
  isFinished = 1;
  if (pthread_mutex_destroy(&ballLock)) {
    perror("Destroy mutex");
    exit(1);
  }
  exit(0);
}

void printOnScreen(int from, int to, int goodOrBad) {
  puts("");
  if (from == 0 && to == 1) {
    if (goodOrBad) {  // dropped || not catched.
      printf("  P[0]  X  P[1].\n");
    } else {  // catched.
      printf("  P[0]  >  P[1].\n");
    }
    printf("                \n");
    printf("  P[3]     P[2].\n");
  } else if (from == 1 && to == 2) {
    printf("  P[0]     P[1].\n");
    if (goodOrBad) {  // dropped || not catched..
      printf("            X   \n");
    } else {  // catched.
      printf("            v   \n");
    }
    printf("  P[3]     P[2].\n");
  } else if (from == 2 && to == 3) {
    printf("  P[0]     P[1].\n");
    printf("                \n");
    if (goodOrBad) {  // dropped || not catched.
      printf("  P[3]  X  P[2].\n");
    } else {  // catched.
      printf("  P[3]  <  P[2].\n");
    }
  } else if (from == 3 && to == 0) {
    printf("  P[0]     P[1].\n");
    if (goodOrBad) {  // dropped || not catched..
      printf("   X            \n");
    } else {  // catched.
      printf("   ^           \n");
    }
    printf("  P[3]     P[2].\n");
  }
  sleep(1);
  system("clear");
}
