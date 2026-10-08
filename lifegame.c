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
  y = (HEIGHT + y) % HEIGHT;
  x = (WIDTH + x) % WIDTH;
  return (y * WIDTH) + x;
}

/*Returns 'is cell alive' on given coordinate.*/
int isAlive(int x, int y) { return buffer[getIndexByXY(x, y)] == '#' ? 1 : 0; }

/*Returns count of alive cells which are near to given cell.*/
int getNearAlives(int x, int y) {
  int alives = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (!j && !i)
        continue;
      alives += isAlive(x + i, y + j);
    }
  }
  return alives;
}

/*Returns 1 if alive. otherwise, returns 0.*/
int getState(int x, int y) {
  int near_alives = getNearAlives(x, y);
  int self_alive = isAlive(x, y);

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
    for (int y = 0; y < HEIGHT; y++) {
      for (int x = 0; x < WIDTH; x++) {
        tempBuffer[getIndexByXY(x, y)] = getState(x, y) ? '#' : ' ';
      }
    }
    memcpy(buffer, tempBuffer, sizeof(tempBuffer));

    // Rendering buffer with line change.
    char string[(WIDTH + 1) * HEIGHT + 1];
    int count = 0;
    fputs("\033[H", stdout);
    for (int y = 0; y < HEIGHT; y++) {
      for (int x = 0; x < WIDTH; x++) {
        string[count++] = buffer[getIndexByXY(x, y)];
      }
      string[count++] = 10;
    }
    string[count] = 0;
    fputs(string, stdout);

    msleep(100);
  }

  return 0;
}
