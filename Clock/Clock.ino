/* Made by Nehtion on Github (18.02.23)
 * This Code is for a 64x64 Matrix
 * The Arduino 2560 is Connected to the RGB-Matrix-P3-64x64
 * and using the Hardware connection from https://www.waveshare.com/wiki/RGB-Matrix-P3-64x64
 * The Real time clock module is: VCC -> 5V; GND -> GND; SDA -> SDA; SCL -> SCL
 */

#include <Wire.h>
#include <DS3231.h> //You can find this libary in the same folder of the code
#include <math.h>

#include "RGBmatrixPanel.h"
#include "bit_bmp.h"
#include "fonts.h"
#include <string.h>
#include <stdlib.h>
#include "Fonts/FreeSerif9pt7b.h"
#include "Fonts/FreeSerifBoldItalic9pt7b.h"
#include "Fonts/RobotoMono_Thin7pt7b.h"
#include "Fonts/FreeSans9pt7b.h"

#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3
#define E   A4


DS3231 clock;
RTCDateTime dt;

RGBmatrixPanel matrix(A, B, C, D, E, CLK, LAT, OE, false, 64);

//Clock variable
#define PI 3.1415926535897932384626433832795
/*
 * By Changing the Center Coordiantes and the radius everything else will 
 * transform as well. Even the numbers will move!
 */
int x_center = 31;
int y_center = 31;
int radius = 19;     
float tAngle;
float tx;
float ty;
float txx;
float tyy;
float sAngle;
float sx;
float sy;
float mAngle;
float mx;
float my;
float hAngle;
float hx;
float hy;
int adj = 45;

void setup() {
  Serial.begin(115200);       //Initialize Serial Monitor
  matrix.begin();             //Initialize matrix
  clock.begin();              //Initialize Clock
  clock.setDateTime(__DATE__, __TIME__);  
  //clock.setDateTime(2023, 2, 18, 14, 00, 00); //Set up Date manually
  aClockSetup();
  //matrix.drawPixel(1,2, matrix.Color333(7, 7, 7));
}

void loop() {
 //matrix.drawPixel(0, 1, matrix.Color333(7, 7, 7));
 //dClock();
 aClock();
}

/*
 * Very simple Digital Clock.
 * Displays the Time in form of HH:MM:SS
 */


void dClock() {
  dt = clock.getDateTime();
  int h = dt.hour;
  int m = dt.minute;
  int s = dt.second;
  matrix.setTextSize(1);
  matrix.setTextWrap(true);
  matrix.setCursor(5, 0); 
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print(h); matrix.print(':'); matrix.print(m); matrix.print(':'); matrix.print(s);
  delay(1000);
  matrix.setCursor(5, 0); 
  matrix.setTextColor(matrix.Color333(0,0,0));
  matrix.print(h); matrix.print(':'); matrix.print(m); matrix.print(':'); matrix.print(s);
}

void aClockSetup() {
  matrix.drawCircle(x_center, y_center, radius+5, matrix.Color333(7,7,7));      //Draw Clock Circle
  for(int i = 0+adj; i < 60+adj; i++) {                                         //Add Minute/second ticks
    tAngle = (6*i) * PI / 180.0;
    tx = x_center + (radius+4) * cos(tAngle);//Serial.print("X: ");Serial.println(tx);
    ty = y_center + (radius+4) * sin(tAngle);//Serial.print("Y: "); Serial.println(ty);
    txx = x_center + (radius+3) * cos(tAngle);//Serial.print("X2: "); Serial.println(txx);
    tyy = y_center + (radius+3) * sin(tAngle);//Serial.print("Y2: "); Serial.println(tyy);
    matrix.drawLine(round(tx),round(ty),round(txx),round(tyy), matrix.Color333(2,7,7));
  }
  for(int i = 0 +adj; i < 12+adj; i++) {                                        //Add Hour ticks
    tAngle = (30*i) * PI / 180.0;
    tx = x_center + (radius+4) * cos(tAngle);//Serial.print("X: ");Serial.println(tx);
    ty = y_center + (radius+4) * sin(tAngle);//Serial.print("Y: ");Serial.println(ty);
    txx = x_center + (radius+1) * cos(tAngle);//Serial.print("X2: ");Serial.println(txx);
    tyy = y_center + (radius+1) * sin(tAngle);//Serial.print("Y2: ");Serial.println(tyy);
    matrix.drawLine(round(tx),round(ty),round(txx),round(tyy), matrix.Color333(7,2,7));
  }
  numbers();
}


void aClock() {
  dt = clock.getDateTime();       //storing time in variables
  int h = dt.hour;
  int m = dt.minute;
  int s = dt.second;
  sAngle = (6*(s+adj)) * PI / 180.0;      //calculating the Radians for seconds
  sx = x_center + radius * cos(sAngle);   //Calculating the X and Y
  sy = y_center + radius * sin(sAngle);   //We Use radians instead of angles since the sinus and cosinus function of arduino only accept radians
  mAngle = (6*(m+adj)) * PI / 180.0;
  mx = x_center + radius * cos(mAngle);
  my = y_center + radius * sin(mAngle);
  hAngle = ((30*(h+adj))+( (6*(m+adj) ) /30 ) ) * PI / 180.0;
  hx = x_center + radius * cos(hAngle);
  hy = y_center + radius * sin(hAngle);                             
  matrix.drawLine(x_center, y_center, sx, sy, matrix.Color333(7, 0, 0));    //drawing handles and removing them after 1 sec
  matrix.drawLine(x_center, y_center, mx, my, matrix.Color333(0, 7, 0));
  matrix.drawLine(x_center, y_center, hx, hy, matrix.Color333(0, 0, 7));
  delay(1000);
  matrix.drawLine(x_center, y_center, sx, sy, matrix.Color333(0, 0, 0));
  matrix.drawLine(x_center, y_center, mx, my, matrix.Color333(0, 0, 0));
  matrix.drawLine(x_center, y_center, hx, hy, matrix.Color333(0, 0, 0));
  
}

/*
 * Displaying the numbers manually since the fonts of the Adafruit GFX libaries, font16.c, ...
 * are too big and can't be displayed properly
 */

void numbers() {
   for(int i = 0 +adj; i < 12+adj; i++) {                                        
    tAngle = (30*i) * PI / 180.0;
    tx = x_center + (radius+10) * cos(tAngle);//Serial.print(i-adj);Serial.print(" X: ");Serial.println(round(tx));
    ty = y_center + (radius+10) * sin(tAngle);//Serial.print(i-adj);Serial.print(" Y: ");Serial.println(round(ty));
    tx = round(tx);
    ty = round(ty);
    if(i-adj==0) { //Number 12
      matrix.drawLine(tx-3,ty-2,tx-2,ty-2,matrix.Color333(7,7,7)); //12: 1
      matrix.drawLine(tx-2,ty-1,tx-2,ty+1,matrix.Color333(7,7,7));
      matrix.drawLine(tx-3,ty+2,tx-1,ty+2,matrix.Color333(7,7,7));
      matrix.drawLine(tx+1,ty-2,tx+3,ty-2,matrix.Color333(7,7,7)); //12: 2
      matrix.drawPixel(tx+3,ty-1,matrix.Color333(7,7,7));
      matrix.drawLine(tx+1,ty,tx+3,ty,matrix.Color333(7,7,7));
      matrix.drawPixel(tx+1,ty+1,matrix.Color333(7,7,7));
      matrix.drawLine(tx+1,ty+2,tx+3,ty+2,matrix.Color333(7,7,7));
    }
    if(i-adj==11) {
      //Number 11
      matrix.drawLine(tx-3,ty-2,tx-2,ty-2,matrix.Color333(7,7,7)); //11: 1
      matrix.drawLine(tx-2,ty-1,tx-2,ty+1,matrix.Color333(7,7,7));
      matrix.drawLine(tx-3,ty+2,tx-1,ty+2,matrix.Color333(7,7,7));
      matrix.drawLine(tx+1,ty-2,tx+2,ty-2,matrix.Color333(7,7,7)); //11: 1
      matrix.drawLine(tx+2,ty-1,tx+2,ty+1,matrix.Color333(7,7,7));
      matrix.drawLine(tx+1,ty+2,tx+3,ty+2,matrix.Color333(7,7,7));
    }
    if(i-adj==10) {
      //Number 10
      matrix.drawLine(tx-3,ty-2,tx-2,ty-2,matrix.Color333(7,7,7)); //10: 1
      matrix.drawLine(tx-2,ty-1,tx-2,ty+1,matrix.Color333(7,7,7));
      matrix.drawLine(tx-3,ty+2,tx-1,ty+2,matrix.Color333(7,7,7));
      matrix.drawLine(tx+1,ty-1,tx+1,ty+1,matrix.Color333(7,7,7)); //10: 0
      matrix.drawPixel(tx+2,ty+2,matrix.Color333(7,7,7));
      matrix.drawLine(tx+3,ty-1,tx+3,ty+1,matrix.Color333(7,7,7));
      matrix.drawPixel(tx+2,ty-2,matrix.Color333(7,7,7));
    }
    if(i-adj==9) {
      //Number 9
      matrix.drawLine(tx-1,ty-2,tx-1,ty,matrix.Color333(7,7,7));
      matrix.drawLine(tx-1,ty+2,tx+1,ty+2,matrix.Color333(7,7,7));
      matrix.drawLine(tx+1,ty-2,tx+1,ty+1,matrix.Color333(7,7,7));
      matrix.drawPixel(tx,ty,matrix.Color333(7,7,7));
      matrix.drawPixel(tx,ty-2,matrix.Color333(7,7,7));
    }
    if(i-adj==8) {
      //Number 8
      matrix.drawLine(tx-1,ty-2,tx-1,ty+2,matrix.Color333(7,7,7));
      matrix.drawLine(tx+1,ty-2,tx+1,ty+2,matrix.Color333(7,7,7));
      matrix.drawPixel(tx,ty-2,matrix.Color333(7,7,7));
      matrix.drawPixel(tx,ty,matrix.Color333(7,7,7));
      matrix.drawPixel(tx,ty+2,matrix.Color333(7,7,7));
    }
    if(i-adj==7) {
      //Number 7
      matrix.drawLine(tx-1,ty-2,tx+1,ty-2,matrix.Color333(7,7,7));
      matrix.drawLine(tx+1,ty-1,tx+1,ty+2,matrix.Color333(7,7,7));
      matrix.drawPixel(tx-1,ty-1,matrix.Color333(7,7,7));
    }
    if(i-adj==6) {
      //Number 6
      matrix.drawLine(tx-1,ty-2,tx-1,ty+2,matrix.Color333(7,7,7));
      matrix.drawLine(tx+1,ty,tx+1,ty+2,matrix.Color333(7,7,7));
      matrix.drawLine(tx-1,ty-2,tx+1,ty-2,matrix.Color333(7,7,7));
      matrix.drawPixel(tx,ty,matrix.Color333(7,7,7));
      matrix.drawPixel(tx,ty+2,matrix.Color333(7,7,7));
    }
    if(i-adj==5) {
      //Number 5
      matrix.drawLine(tx-1,ty-2,tx+1,ty-2,matrix.Color333(7,7,7)); 
      matrix.drawPixel(tx-1,ty-1,matrix.Color333(7,7,7));
      matrix.drawLine(tx-1,ty,tx+1,ty,matrix.Color333(7,7,7));
      matrix.drawPixel(tx+1,ty+1,matrix.Color333(7,7,7));
      matrix.drawLine(tx-1,ty+2,tx+1,ty+2,matrix.Color333(7,7,7));
    }
    if(i-adj==4) {
      //Number 4
      matrix.drawLine(tx-1,ty+1,tx+2,ty+1,matrix.Color333(7,7,7)); 
      matrix.drawPixel(tx,ty-1,matrix.Color333(7,7,7));
      matrix.drawLine(tx+1,ty-2,tx+1,ty+2,matrix.Color333(7,7,7));
      matrix.drawPixel(tx-1,ty,matrix.Color333(7,7,7));
    }
    if(i-adj==3) {
      //Number 3
      matrix.drawLine(tx-1,ty+2,tx+1,ty+2,matrix.Color333(7,7,7)); 
      matrix.drawPixel(tx+1,ty-1,matrix.Color333(7,7,7));
      matrix.drawLine(tx-1,ty,tx+1,ty,matrix.Color333(7,7,7));
      matrix.drawPixel(tx+1,ty+1,matrix.Color333(7,7,7));
      matrix.drawLine(tx-1,ty-2,tx+1,ty-2,matrix.Color333(7,7,7));
    }
    if(i-adj==2) {
      //Number 2
      matrix.drawLine(tx-1,ty-2,tx+1,ty-2,matrix.Color333(7,7,7));
      matrix.drawPixel(tx+1,ty-1,matrix.Color333(7,7,7));
      matrix.drawLine(tx-1,ty,tx+1,ty,matrix.Color333(7,7,7));
      matrix.drawPixel(tx-1,ty+1,matrix.Color333(7,7,7));
      matrix.drawLine(tx-1,ty+2,tx+1,ty+2,matrix.Color333(7,7,7));
    }
    if(i-adj==1) {
      //Number 1
      matrix.drawLine(tx-1,ty-2,tx,ty-2,matrix.Color333(7,7,7));
      matrix.drawLine(tx,ty-1,tx,ty+1,matrix.Color333(7,7,7));
      matrix.drawLine(tx-1,ty+2,tx+1,ty+2,matrix.Color333(7,7,7));
    }
   }
}

void screen_clear() {
  matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));
}
