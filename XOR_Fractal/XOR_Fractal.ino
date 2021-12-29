// Simple XOR fractal with sound //

#include "SPI.h"
#include "GD.h"
#include "randreg.h"

#define WIDTH  256
#define HEIGHT 256

  int d;
  byte t;

#define RANDOM_RGB() RGB(rand()%256,rand()%256,rand()%256)

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
    int x =     72 + 16 * ((i >> 4) & 15);
    int y =     22 + 16 * (i & 15); 
    int image = i & 63;
    int pal =   3-(i >> 6);
    GD.sprite(i, x, y, image, 0x8 | (pal << 1), 0,0);
  }
  
  GD.fill(RAM_SPRIMG, 0, 16384);
  GD.wr16(PALETTE4A, RGB(0,0,0));
  GD.wr16(PALETTE4A + 2, RANDOM_RGB());
  GD.wr16(PALETTE4A + 4, RANDOM_RGB());
  GD.wr16(PALETTE4A + 6, RANDOM_RGB());
  GD.putstr(16, 1, "Simple XOR fractal");
  GD.putstr(16, 36, "Created by JLS 2021");

  d = rand()%256;
  
}

void loop(){ 

  for(int y = 0; y < HEIGHT; y++) {
        
    for(int x = 0; x < WIDTH; x++) {
         
      t = (x^y)%d;
  
      setpixel(x, y, replicate(t%3));
      
      GD.wr(SAMPLE_L+1,t);
      GD.wr(SAMPLE_R+1,t);

    }
      
  }    
        
  d++; 
   
}
