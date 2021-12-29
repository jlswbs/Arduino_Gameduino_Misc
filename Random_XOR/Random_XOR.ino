// Random XOR fractal //

#include "SPI.h"
#include "GD.h"
#include "randreg.h"

#define WIDTH 256
#define HEIGHT 256

#define RANDOM_RGB() RGB(rand()%256,rand()%256,rand()%256)

  int x, y;
   
byte replicate(byte color){ return (color << 6) | (color << 4) | (color << 2) | color; }
           
void setpixel(byte x, byte y, byte color){
  unsigned int addr = RAM_SPRIMG|(x & 0xf)|(y << 4)|((x & 0x30)<<8);
  byte mask = 0xc0 >> ((x >> 5) & 6);
  GD.wr(addr, (GD.rd(addr) & ~mask) | (color & mask));
}

void setup(){
  
  int i;
  GD.begin();
  GD.microcode(randreg_code, sizeof(randreg_code));
  srand(GD.rd(0x280E));
  GD.ascii();
  
  for (i = 0; i < 256; i++) {
    int x = 72 + 16 * ((i >> 4) & 15);
    int y = 22 + 16 * (i & 15); 
    int image = i & 63;
    int pal = 3-(i >> 6);
    GD.sprite(i, x, y, image, 0x8 | (pal << 1), 0,0);
  }
  
  GD.fill(RAM_SPRIMG, 0, 16384);
  GD.wr16(PALETTE4A, RGB(0,0,0));
  GD.wr16(PALETTE4A + 2, RANDOM_RGB());
  GD.wr16(PALETTE4A + 4, RANDOM_RGB());
  GD.wr16(PALETTE4A + 6, RANDOM_RGB());
  GD.putstr(16, 1, "Random XOR fractal");
  GD.putstr(16, 36, "Created by JLS 2021");
    
}


void loop(){ 
    
  x = rand()%WIDTH;
  y = rand()%HEIGHT;

  setpixel (x, y, replicate((x^y)%3));
    
}
