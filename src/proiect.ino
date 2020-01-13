#include <Servo.h>
// Pinii motor 1
#define mpin00 5
#define mpin01 6
// Pinii motor 2
#define mpin10 3
#define mpin11 11

#define INAINTE 0
#define INAPOI 1
#define STANGA 2
#define DREAPTA 3

int matrice_initiala[8][8] =
{
  {-1,-1,-1,-1,-1,-1,-1,-1},
  {-1, 0, 0, 0, 0, 0, 0,-1},
  {-1, 0,-1,-1,-1,-1, 0,-1},
  {-1, 0,-1, 0,-1, 0, 0,-1},
  {-1, 0,-1, 0, 0, 0,-1,-1},
  {-1, 0, 0,-1,-1, 0, 0,-1},
  {-1,-1, 0, 0, 0, 0, 0,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1}
};
int matrice[8][8];
int drumx[64] = {0};
int drumy[64] = {0};
int x = 6, y = 6;
int gx = 1, gy = 1;

Servo srv;

void setup() {
 // configurarea pinilor motor ca iesire, initial valoare 0
 digitalWrite(mpin00, 0);
 digitalWrite(mpin01, 0);
 digitalWrite(mpin10, 0);
 digitalWrite(mpin11, 0);
 pinMode (mpin00, OUTPUT);
 pinMode (mpin01, OUTPUT);
 pinMode (mpin10, OUTPUT);
 pinMode (mpin11, OUTPUT);
 // pin LED
 pinMode(13, OUTPUT);
 
 //initializam matricea de cautare
 for(int i = 0; i < 8; i++){
   for(int j = 0; j < 8; j++){
      matrice[i][j] = matrice_initiala[i][j];
   }
 }
 gasesteDrum();
}

// Funcție pentru controlul unui motor
// Intrare: pinii m1 și m2, direcția și viteza
void StartMotor (int m1, int m2, int forward, int speed)
{

 if (speed==0) // oprire
 {
 digitalWrite(m1, 0);
 digitalWrite(m2, 0);
 }
 else
 {
 if (forward)
 {
 digitalWrite(m2, 0);
 analogWrite(m1, speed); // folosire PWM
 }
 else
 {
 digitalWrite(m1, 0);
 analogWrite(m2, speed);
 }
 }
}



// Funcție de siguranță
// Execută oprire motoare, urmată de delay
void delayStopped(int ms)
{
 StartMotor (mpin00, mpin01, 0, 0);
 StartMotor (mpin10, mpin11, 0, 0);
 delay(ms);
}
void miscare(int directie){
 if(directie == INAINTE){
   // Acum se merge inainte
   StartMotor (mpin00, mpin01, 1, 128);
   StartMotor (mpin10, mpin11, 1, 128);

 }
 else if(directie == INAPOI){
   // Acum se merge inapoi
   StartMotor (mpin00, mpin01, 0, 128);
   StartMotor (mpin10, mpin11, 0, 128);
 }
 else if(directie == STANGA){
   // Acum se merge la stanga
   StartMotor (mpin00, mpin01, 0, 128);
   StartMotor (mpin10, mpin11, 1, 128);

 }
 else if(directie == DREAPTA){
   // Acum se merge la dreapta
   StartMotor (mpin00, mpin01, 1, 128);
   StartMotor (mpin10, mpin11, 0, 128);
 }
 delayStopped(500); // Oprim motorul
}


void gasesteDrum(){
  int pozitie = 0;
  while(x != gx && y != gy){
    if(x > 0 && matrice[x-1][y] == 0){
      matrice[x-1][y] = matrice[x][y] + 1;
      x = x-1;
    }
    else if(x < 7 && matrice[x+1][y] == 0){
      matrice[x+1][y] = matrice[x][y] + 1;
      x = x+1;
    }
    else if(y > 0 && matrice[x][y-1] == 0){
      matrice[x][y-1] = matrice[x][y] + 1;
      y = y-1;
    }
    else if(y < 7 && matrice[x][y+1] == 0){
      matrice[x][y+1] = matrice[x][y] + 1;
      y = y+1;
    }
  }
  int dimensiune = matrice[gx][gy];
  drumx[dimensiune] = x;
  drumy[dimensiune] = y;
  while(matrice[x][y] !=0){
    if(x > 0 && matrice[x-1][y] == matrice[x][y] - 1){
      x = x-1;
    }
    else if(x < 7 && matrice[x+1][y] == matrice[x][y] - 1){
      x = x+1;
    }
    else if(y > 0 && matrice[x][y-1] == 0){
      y = y-1;
    }
    else if(y < 7 && matrice[x][y+1] == 0){
      y = y+1;
    }
    dimensiune--;
    drumx[dimensiune] = x;
    drumy[dimensiune] = y;
  }
}


void loop() {
  int obstacol = 0;
  int pozitie = 0;
  int directie = INAINTE;
  //obstacol = digitalRead(PIN_SENZOR);
  if(obstacol){
    //notam obstacolul in matricea initiala
    matrice_initiala[drumx[pozitie+1]][drumy[pozitie+1]] = -1;
    //reinitializam matricea de cautare
     for(int i = 0; i < 8; i++){
       for(int j = 0; j < 8; j++){
          matrice[i][j] = matrice_initiala[i][j];
       }
     }
     //cautam noul drum
     gasesteDrum();
  }
  else{
    if(drumx[pozitie] = gx && drumy[pozitie] == gy)
      directie = -1;//ne oprim din mers
    if(directie == INAINTE){
      if(drumx[pozitie+1] < drumx[pozitie]){
        pozitie = pozitie+1;
        miscare(INAINTE);
      }
      else if(drumx[pozitie+1] > drumx[pozitie]){
        pozitie = pozitie+1;
        miscare(STANGA);
        miscare(STANGA);
        miscare(INAINTE);
        directie = INAPOI;
      }
      else if(drumy[pozitie+1] < drumy[pozitie]){
        pozitie = pozitie+1;
        miscare(STANGA);
        miscare(INAINTE);
        directie = STANGA;
      }
      else if(drumy[pozitie+1] > drumy[pozitie]){
        pozitie = pozitie+1;
        miscare(DREAPTA);
        miscare(INAINTE);
        directie = DREAPTA;
      }
    }
    else if(directie == INAPOI){
      if(drumx[pozitie+1] < drumx[pozitie]){
        pozitie = pozitie+1;
        miscare(STANGA);
        miscare(STANGA);
        miscare(INAINTE);
        directie = INAINTE;
      }
      else if(drumx[pozitie+1] > drumx[pozitie]){
        pozitie = pozitie+1;
        miscare(INAINTE);
      }
      else if(drumy[pozitie+1] < drumy[pozitie]){
        pozitie = pozitie+1;
        miscare(DREAPTA);
        miscare(INAINTE);
        directie = STANGA;
      }
      else if(drumy[pozitie+1] > drumy[pozitie]){
        pozitie = pozitie+1;
        miscare(STANGA);
        miscare(INAINTE);
        directie = DREAPTA;
      }
    }
    else if(directie == STANGA){
      if(drumx[pozitie+1] < drumx[pozitie]){
        pozitie = pozitie+1;
        miscare(DREAPTA);
        miscare(INAINTE);
        directie = INAINTE;
      }
      else if(drumx[pozitie+1] > drumx[pozitie]){
        pozitie = pozitie+1;
        miscare(STANGA);
        miscare(INAINTE);
        directie = INAPOI;
      }
      else if(drumy[pozitie+1] < drumy[pozitie]){
        pozitie = pozitie+1;
        miscare(INAINTE);
      }
      else if(drumy[pozitie+1] > drumy[pozitie]){
        pozitie = pozitie+1;
        miscare(STANGA);
        miscare(STANGA);
        miscare(INAINTE);
        directie = DREAPTA;
      }
    }
    else if(directie == DREAPTA){
      if(drumx[pozitie+1] < drumx[pozitie]){
        pozitie = pozitie+1;
        miscare(STANGA);
        miscare(INAINTE);
        directie = INAINTE;
      }
      else if(drumx[pozitie+1] > drumx[pozitie]){
        pozitie = pozitie+1;
        miscare(DREAPTA);
        miscare(INAINTE);
        directie = INAPOI;
      }
      else if(drumy[pozitie+1] < drumy[pozitie]){
        pozitie = pozitie+1;
        miscare(STANGA);
        miscare(STANGA);
        miscare(INAINTE);
        directie = STANGA;
      }
      else if(drumy[pozitie+1] > drumy[pozitie]){
        pozitie = pozitie+1;
        miscare(INAINTE);
      }
    }
  }
} 
