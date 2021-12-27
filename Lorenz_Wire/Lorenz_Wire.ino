// Lorenz chaotic attractor - wire mode //

#include <SPI.h>
#include "GD.h"

#include "wireframe.h"
#include "eraser.h"

  float s = 10;
  float r = 28;
  float b = 8/3;

  float x = 0;
  float y = 1;
  float z = 0;

  float xn = x;
  float yn = y;
  float zn = z;

  float dt = 0.005;
  

byte replicate(byte color){ return (color << 6) | (color << 4) | (color << 2) | color; }

#define BLACK RGB(0,0,0)
#define WHITE RGB(255,255,255)

class PlotterClass
{
public:
  void begin();
  void line(byte x0, byte y0, byte x1, byte y1);
  void show();
private:
  byte flip;
  byte plotting;
  void erase();
  void waitready();
};

PlotterClass Plotter;

void PlotterClass::waitready(){ while (GD.rd(COMM+7));}

void PlotterClass::erase(){
  
  byte color = flip ? 1 : 2;

  plotting = 0;
  GD.wr(J1_RESET, 1);
  GD.wr(COMM+7, 1);
  GD.wr(COMM+8, replicate(color ^ 3));
  GD.microcode(eraser_code, sizeof(eraser_code));
  
}

void PlotterClass::begin(){

  unsigned int i;
  for (i = 0; i < 256; i++) {
    int x =     72 + 16 * ((i >> 4) & 15);
    int y =     22 + 16 * (i & 15);
    int image = i & 63;     /* image 0-63 */
    int pal =   3 - (i >> 6);   /* palettes bits in columns 3,2,1,0 */
    GD.sprite(i, x, y, image, 0x8 | (pal << 1), 0);
  }

  flip = 0;
  plotting = 0;
  erase();
  show();
}

void PlotterClass::show(){
  
  waitready();
  if (flip == 1) {
    GD.wr16(PALETTE4A, BLACK);
    GD.wr16(PALETTE4A + 2, WHITE);
    GD.wr16(PALETTE4A + 4, BLACK);
    GD.wr16(PALETTE4A + 6, WHITE);
  } else {
    GD.wr16(PALETTE4A, BLACK);
    GD.wr16(PALETTE4A + 2, BLACK);
    GD.wr16(PALETTE4A + 4, WHITE);
    GD.wr16(PALETTE4A + 6, WHITE);
  }
  flip ^= 1;
  erase();
}

void PlotterClass::line(byte x0, byte y0, byte x1, byte y1){
  
  byte swap;
  #define SWAP(a, b) (swap = (a), (a) = (b), (b) = swap)

  byte steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    SWAP(x0, y0);
    SWAP(x1, y1);
  }
  if (x0 > x1) {
    SWAP(x0, x1);
    SWAP(y0, y1);
  }
  int deltax = x1 - x0;
  int deltay = abs(y1 - y0);
  int error = deltax / 2;
  char ystep;
  if (y0 < y1)  
    ystep = 1;
  else
    ystep = -1;
  byte x;
  byte y = y0;

  waitready();
  if (!plotting) {
    GD.microcode(wireframe_code, sizeof(wireframe_code));
    plotting = 1;
    byte color = flip ? 1 : 2;
    GD.wr(COMM+8, color << 6);
  }
  GD.__wstart(COMM+0);
  SPI.transfer(x0);
  SPI.transfer(y0);
  SPI.transfer(x1);
  SPI.transfer(y1);
  SPI.transfer(steep);
  SPI.transfer(deltax);
  SPI.transfer(deltay);
  SPI.transfer(ystep);
  GD.__end();
}

void setup(){
  
  GD.begin();
  GD.ascii();
  GD.putstr(14, 1, "Lorenz Chaotic Attractor");
  GD.putstr(16, 36, "Created by JLS 2021");
  Plotter.begin();

}


void loop(){
  
  for (int i=0; i<2048; i++){
    
    xn = s*(y-x);
    yn = (r*x)-y-(x*z);
    zn = (x*y)-(b*z);

    x = x+xn*dt;
    y = y+yn*dt;
    z = z+zn*dt;
  
    Plotter.line(127+(5*x), 127+(4*y), 128+(5*x), 128+(4*y));
  
  }
  
  Plotter.show();
  
}
