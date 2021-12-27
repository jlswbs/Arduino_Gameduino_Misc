// 5-state 1D cellular automata //

#include "SPI.h"
#include "GD.h"
#include "randreg.h"

#define WIDTH   256
#define HEIGHT  256
#define RND   11
#define VAL   15
#define COL   5

  uint8_t coll;
  uint8_t parent[WIDTH]; 
  uint8_t child[WIDTH];
  int count;
  int a[VAL];

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

  for(int i = 0; i < VAL; i++) a[i] = rand()%RND;

  for(int i = 0; i < WIDTH; i++){
  
    parent[i] = rand()%COL;
    child[i] = 0;

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
  
  GD.putstr(11, 1, "5-state 1D cellular automata");
  GD.putstr(16, 36, "Created by JLS 2021");
 
}


void loop(){

  rndrule();

  for (int y = 0; y < HEIGHT; y++) {
 
    for (int x = 0; x < WIDTH; x++) {
          
      if (x == 0) count = parent[WIDTH-1] + parent[0] + parent[1];
      else if (x == WIDTH-1) count = parent[WIDTH-2] + parent[WIDTH-1] + parent[0];
      else count = parent[x-1] + parent[x] + parent[x+1];        
            
      if(count == a[0] || count == a[1] || count == a[2]) child[x] = 0;
      if(count == a[3] || count == a[4] || count == a[5]) child[x] = 1;
      if(count == a[6] || count == a[7] || count == a[8]) child[x] = 2;
      if(count == a[9] || count == a[10] || count == a[11]) child[x] = 3;
      if(count == a[12] || count == a[13] || count == a[14]) child[x] = 4;
               
      if (child[x] == 0) coll = 0;
      if (child[x] == 1) coll = 1;
      if (child[x] == 2) coll = 2;
      if (child[x] == 3) coll = 1;
      if (child[x] == 4) coll = 2;
      
      setpixel (x, y, replicate((child[x] + 1) & 3));
                                                   
    }
                                                 
    memcpy(parent, child, WIDTH);

  }

}
