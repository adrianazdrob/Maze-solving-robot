#include "QueueList.h"
int m[20][20] =
{
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 1, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 0, 0}
};

int startX = 0;
int startY = 2;
int destX = 8;
int destY = 1;
int leftLED = 7;
int rightLED = 11;
int upLED = 12;
int downLED = 13;

enum MOVES {UP, DOWN, LEFT, RIGHT, null};
enum STATES {WHITE, GREY, BLACK};

bool validMove(int r, int c) {
  if (r >= 0 && r <= 8) {
    if (c >= 0 && c <= 7) {
      return true;
    }
  }
  return false;
}

struct Queuepoint {
  int x;
  int y;
  MOVES move;
};

int rowMoves[] = { -1, 0, 0, 1 };
int colMoves[] = { 0, -1, 1, 0 };
MOVES moves[100];
STATES states[10][10];
int indexMoves = 0;

void solver() {
  if (m[startX][startY] == 0 || m[destX][destY] == 0) {
    return;
  }
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      states[i][j] = WHITE;
    }
  }
  states[startX][startY] = GREY;

  QueueList <Queuepoint> q;
  Queuepoint src = { startX, startY };
  q.push(src);

  while (!q.isEmpty()) {
    Queuepoint aux = q.peek();
    if (aux.x == destX && aux.y == destY) {
      return;
    }
    q.pop();
    int ok = 1;
    for (int i = 0; i < 4; i++) {
      int row = aux.x + rowMoves[i];
      int col = aux.y + colMoves[i];
      if (validMove(row, col) && m[row][col] == 1) {
        if (states[row][col] == WHITE) {
          switch (i) {
            case 0:
              moves[indexMoves++] = UP;
              break;
            case 1:
              moves[indexMoves++] = LEFT;
              break;
            case 2:
              moves[indexMoves++] = RIGHT;
              break;
            case 3:
              moves[indexMoves++] = DOWN;
            default:
              break;
          }
          ok = 0;
          states[row][col] = GREY;
          Queuepoint adj = { row, col, moves[indexMoves - 1]};
          q.push(adj);
        }
      }
    }
    states[aux.x][aux.y] = BLACK;
    
    if (ok == 1) {
      if (aux.move == UP) moves[indexMoves++] = DOWN;
      if (aux.move == DOWN) moves[indexMoves++] = UP;
      if (aux.move == LEFT) moves[indexMoves++] = RIGHT;
      if (aux.move == RIGHT) moves[indexMoves++] = LEFT;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(leftLED, OUTPUT);
  pinMode(rightLED, OUTPUT);
  pinMode(upLED, OUTPUT);
  pinMode(downLED, OUTPUT);

  digitalWrite(leftLED, LOW);
  digitalWrite(rightLED, LOW);
  digitalWrite(upLED, LOW);
  digitalWrite(downLED, LOW);

  up();
  delay(150);
  down();
  delay(150);
  left();
  delay(150);
  right();
  delay(150);
  digitalWrite(rightLED, LOW);
  delay(1000);

  Serial.begin(9600);
  solver();
  Serial.print("Number of moves: ");
  Serial.println(indexMoves);
}

int i = 0;
void loop() {

  if (moves[i] == UP) {
    Serial.println("UP");
    up();
  }
  if (moves[i] == DOWN) {
    Serial.println("DOWN");
    down();
  }
  if (moves[i] == LEFT) {
    Serial.println("LEFT");
    left();
  }
  if (moves[i] == RIGHT) {
    Serial.println("RIGHT");
    right();
  }
  
  delay(300);
  i++;
  off();
  delay(200);

  if (i == indexMoves) {
    off();
    delay(10000);
    i = 0;
    Serial.println("STOP");
    Serial.println("");
    Serial.println("");
  }
}

void off() {
  digitalWrite(leftLED, LOW);
  digitalWrite(rightLED, LOW);
  digitalWrite(upLED, LOW);
  digitalWrite(downLED, LOW);
}

void left() {
  digitalWrite(leftLED, HIGH);
  digitalWrite(rightLED, LOW);
  digitalWrite(upLED, LOW);
  digitalWrite(downLED, LOW);
}

void up() {
  digitalWrite(leftLED, LOW);
  digitalWrite(rightLED, LOW);
  digitalWrite(upLED, HIGH);
  digitalWrite(downLED, LOW);
}

void right() {
  digitalWrite(leftLED, LOW);
  digitalWrite(rightLED, HIGH);
  digitalWrite(upLED, LOW);
  digitalWrite(downLED, LOW);
}

void down() {
  digitalWrite(leftLED, LOW);
  digitalWrite(rightLED, LOW);
  digitalWrite(upLED, LOW);
  digitalWrite(downLED, HIGH);
}
