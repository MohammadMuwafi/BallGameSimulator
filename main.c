#include "local.h"

int main(int argc, char const *argv[]) {
  system("clear");

  for (int i = 0; i < NUM_OF_PLAYERES; i++) {
    ThreadParam *threadParam = malloc(sizeof(threadParam));
    if (threadParam == NULL) {
      perror("error in malloc");
      exit(1);
    }
    threadParam->playerNumber = i;
    if (pthread_create(&players[i], NULL, (void *)playerRoung,
                       (void *)(threadParam))) {
      perror("Create thread");
      exit(1);
    }
  }
  for (int i = 0; i < NUM_OF_PLAYERES; i++) {
    if (pthread_join(players[i], NULL)) {
      perror("Join thread");
      exit(1);
    }
  }

  return 0;
}

void *playerRoung(void *arg) {
  ThreadParam param = *((ThreadParam *)arg);
  int currentPlayerNumber = param.playerNumber;
  int nextPlayerNumber = (param.playerNumber + 1) % NUM_OF_PLAYERES;

  while (true) {
    sleep(1);
    if (pthread_mutex_lock(&ballLock)) {
      perror("Error in mutex");
      exit(1);
    }

    // check if the game was ended.
    for (int i = 0; i < NUM_OF_PLAYERES; i++) {
      if (playerErrors[i] > 4 && !isFinished) {
        puts("Terminated!");
        if (pthread_mutex_unlock(&ballLock)) {
          perror("Error in mutex");
          exit(1);
        }
        exitProgram();
      }
    }
    // the ball with someone.
    if (ball[currentPlayerNumber] == 1) {
      int notCatched = (rand() % 100) <= 40;
      int dropped = (rand() % 100) <= 10;
      playerErrors[currentPlayerNumber] += (notCatched + dropped);
      printOnScreen(currentPlayerNumber, nextPlayerNumber, notCatched || dropped);
      ball[nextPlayerNumber] = ball[currentPlayerNumber]--;
    }

    if (pthread_mutex_unlock(&ballLock)) {
      perror("Error in mutex");
      exit(1);
    }
  }
}