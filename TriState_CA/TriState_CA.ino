// 3-state 1D cellular automata //

#include "SPI.h"
#include "GD.h"
#include "randreg.h"

#define WIDTH   256
#define HEIGHT  256
#define RND     7

  uint8_t coll;
  uint8_t parent[WIDTH]; 
  uint8_t child[WIDTH];
  int count;
  int a, b, c, d, e, f, g, h, i;

#define RANDOM_RGB() RGB(rand()%256,rand()%256,rand()%256)

byte replicate(byte color){ return (color << 6) | (color << 4) | (color << 2) | color; }
           
void setpixel(byte x, byte y, byte color){
  unsigned int addr = RAM_SPRIMG|(x & 0xf)|(y << 4)|((x & 0x30)<<8);
  byte mask = 0xc0 >> ((x >> 5) & 6);
  GD.wr(addr, (GD.rd(addr) & ~mask) | (color & mask));
}

void rndrule(){

  GD.fill(RAM_SPRIMG, 0, 16384);
  GD.wr16(PALETTE4A, RGB(25,25,25));
  GD.wr16(PALETTE4A + 2, RANDOM_RGB());
  GD.wr16(PALETTE4A + 4, RANDOM_RGB());
  GD.wr16(PALETTE4A + 6, RANDOM_RGB());

  a = rand()%RND;
  b = rand()%RND;
  c = rand()%RND;
  d = rand()%RND;
  e = rand()%RND;
  f = rand()%RND;
  g = rand()%RND;
  h = rand()%RND;
  i = rand()%RND;

  for(int x = 0; x < WIDTH; x++){
  
    parent[x] = rand()%4;
    child[x] = 0;

  }

}

void setup(){
  
  GD.begin();
  GD.microcode(randreg_code, sizeof(randreg_code));
  srand(GD.rd(0x280E));
  GD.ascii();
  
  for (int i = 0; i < 256; i++) {
    int x = 72 + 16 * ((i >> 4) & 15);
    int y = 22 + 16 * (i & 15); 
    int image = i & 63;
    int pal = 3-(i >> 6);
    GD.sprite(i, x, y, image, 0x8 | (pal << 1), 0,0);
  }
  
  GD.putstr(11, 1, "3-state 1D cellular automata");
  GD.putstr(16, 36, "Created by JLS 2021");
 
}


void loop(){

  rndrule();

  for (int y = 0; y < HEIGHT; y++) {
 
    for (int x = 0; x < WIDTH; x++) {
          
      if (x == 0) count = parent[WIDTH-1] + parent[0] + parent[1];
      else if (x == WIDTH-1) count = parent[WIDTH-2] + parent[WIDTH-1] + parent[0];
      else count = parent[x-1] + parent[x] + parent[x+1];        
            
      if (count == a || count == b || count == c) child[x] = 0;
      if (count == d || count == e || count == f) child[x] = 1;
      if (count == g || count == h || count == i) child[x] = 2;
      
      setpixel (x, y, replicate((child[x] + 1) & 3));
                                                   
    }
                                                 
    memcpy(parent, child, WIDTH);

  }

}
