#include <bits/time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <time.h>

#define msleep(ms)                                                             \
  {                                                                            \
    long long seconds = 0;                                                     \
    if (ms >= 1000) {                                                          \
      seconds = ms / 1000;                                                     \
    }                                                                          \
    struct timespec ts = {seconds, (ms % 1000) * 1000 * 1000};                 \
    nanosleep(&ts, NULL);                                                      \
  }

#define WIDTH 200
#define HEIGHT 50

int rollRandom(int start, int end) {
  return start + (rand() % (end - start + 1));
}

char buffer[WIDTH * HEIGHT];
char tempBuffer[WIDTH * HEIGHT];

/*Returns index of list by given x,y.*/
int getIndexByXY(int x, int y) {
  y = y >= 0 ? (y < HEIGHT ? y : y - HEIGHT) : HEIGHT + y;
  x = x >= 0 ? (x < WIDTH ? x : x - WIDTH) : WIDTH + x;
  return (y * WIDTH) + x;
}

/*Returns 'is cell alive' on given coordinate.*/
int isAlive(int idx) { return buffer[idx] == '#' ? 1 : 0; }

/*Returns count of alive cells which are near to given cell.*/
int getNearAlives(int idx) {
  int x = idx % WIDTH;
  int y = idx / WIDTH;

  int alives = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (!j && !i)
        continue;
      alives += isAlive(getIndexByXY(x + i, y + j));
    }
  }
  return alives;
}

/*Returns 1 if alive. otherwise, returns 0.*/
int getState(int idx) {
  int near_alives = getNearAlives(idx);
  int self_alive = isAlive(idx);

  return near_alives == 3 ? 1 : self_alive && near_alives == 2;
}

int main() {
  srand(time(NULL));

  // Clearing console.
  printf("\033[H\033[2J");
  fflush(stdout);

  // Clearing buffer with space;
  for (int i = 0; i < WIDTH * HEIGHT; i++) {
    buffer[i] = ' ';
  }

  // Spread random alive cells.
  for (int i = 0; i < 1000; i++) {
    buffer[getIndexByXY(rollRandom(0, WIDTH - 1), rollRandom(0, HEIGHT - 1))] =
        '#';
  }

  while (1) {
    // Updating old buffer using tempBuffer
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
      tempBuffer[i] = getState(i) ? '#' : ' ';
    }
    memcpy(buffer, tempBuffer, sizeof(tempBuffer));

    // Rendering buffer with line change.
    printf("\033[H");
    for (int i = 1; i <= WIDTH * HEIGHT; i++) {
      printf("%c", buffer[i - 1]);
      if (i % WIDTH == 0)
        printf("\n");
    }
    fflush(stdout);
    msleep(100);
  }

  return 0;
}
