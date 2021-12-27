// StarWars cellular automata //

#include "SPI.h"
#include "GD.h"
#include "randreg.h"

#define CONST 11
#define WIDTH 256/CONST
#define HEIGHT 256/CONST
#define MAX_GEN 128

  byte current [WIDTH][HEIGHT];
  byte next [WIDTH][HEIGHT];
  byte alive_counts [WIDTH][HEIGHT];
  byte tmp[WIDTH][HEIGHT];
  
  int ALIVE = 3;
  int DEATH_1 = 2;
  int DEAD = 0;
  char msg[30];
  int x,y,nx,ny,generations;
  
   
byte replicate(byte color){ return (color << 6) | (color << 4) | (color << 2) | color; }
           
void setpixel(byte x, byte y, byte color){
  unsigned int addr = RAM_SPRIMG|(x & 0xf)|(y << 4)|((x & 0x30)<<8);
  byte mask = 0xc0 >> ((x >> 5) & 6);
  GD.wr(addr, (GD.rd(addr) & ~mask) | (color & mask));
}

void drawfillbox(byte x, byte y, byte color){
  int nx ,ny;
  for (ny = 0; ny < CONST-3; ny++) {
    for (nx = 0; nx < CONST-3; nx++) setpixel (2+(CONST*x)+nx, 2+(CONST*y)+ny, replicate((color+1) & 3));
  }
}


void setup(){
  
  int i;
  GD.begin();
  GD.microcode(randreg_code, sizeof(randreg_code));
  GD.ascii();
  for (i = 0; i < 256; i++) {
  int x = 72 + 16 * ((i >> 4) & 15);
  int y = 22 + 16 * (i & 15); 
  int image = i & 63;
  int pal =   3-(i >> 6);
  GD.sprite(i, x, y, image, 0x8 | (pal << 1), 0,0);}
  GD.fill(RAM_SPRIMG, 0, 16384);
  GD.wr16(PALETTE4A, RGB(0,0,0));
  GD.wr16(PALETTE4A + 2, RGB(255,255,0));
  GD.wr16(PALETTE4A + 4, RGB(0,255,255));
  GD.wr16(PALETTE4A + 6, RGB(255,255,255));
  GD.putstr(12, 1, "StarWars cellular automata");
  GD.putstr(16, 36, "Created by JLS 2021");
    
  randseed();
}


void loop(){
   
  step();
      
  GD.fill(RAM_SPRIMG, 0, 16384);
      
  draw_type(50,100, 2);
  draw_type(2,49, 1);
  draw_type(0,1, 0);

  GD.putstr(41, 36, msg);
  sprintf(msg, "%3d gens", generations);

  if (generations > MAX_GEN) {
    generations = 0;
    randseed();
  }

  generations++;

  GD.waitvblank();
}

void randseed() {
  
  x = 0;
  y = 0;

  for (x = 0; x < WIDTH; x++) {
    for (y = 0; y < HEIGHT; y++) {
    
      current[x][y] = GD.rd(0x280E)%5;
      alive_counts[x][y] = 0;  
    }
  }

  GD.fill(RAM_SPRIMG, 0, 16384);
}


void step() {
 
  for (x = 0; x < WIDTH; x++) {
  
    for (y = 0; y < HEIGHT; y++) {
    
      int count = 0;
      int next_val;
      int mx = WIDTH-1;
      int my = HEIGHT-1;
      int self = current[x][y];
    
    for (nx = x-1; nx <= x+1; nx++) {
  
      for (ny = y-1; ny <= y+1; ny++) {
    
        if (nx == x && ny == y) continue;
      
        if (current[wrap(nx, mx)][wrap(ny, my)] == ALIVE) count++;
      
      }   
    }  

    int neighbors = count;
    
    if (self == ALIVE) next_val = ((neighbors == 3) || (neighbors == 4) || (neighbors == 5)) ? ALIVE : DEATH_1;
  
    else if (self == DEAD) next_val = (neighbors == 2) ? ALIVE : DEAD;
  
    else next_val = self-1;
   
    next[x][y] = next_val;
  
    if (next_val == ALIVE) {
    
      if ((alive_counts[x][y]+1) <= 100) alive_counts[x][y] = (int)(alive_counts[x][y]+1);
        
      } else if (next_val == DEAD) alive_counts[x][y] = 0;
      }
    }
    
    for (x = 0; x < WIDTH; x++) {
      for (y = 0; y < HEIGHT; y++) {
        tmp[x][y] = current[x][y];
        current[x][y] = next[x][y];
        next[x][y] = tmp[x][y];
      }
    }
}
  

  
int wrap(int v, int m) {

  if (v < 0) return v + m;
  else if (v >= m) return v - m;
  else return v;
}
  
  
void draw_type(int min_alive, int max_alive, int r) {  
    
  for (x = 0; x < WIDTH; x++) { 
    for (y = 0; y < HEIGHT; y++) {
      int self = current[x][y];  
      if (self == DEAD) continue;  
      int alive = alive_counts[x][y];
      if (alive < min_alive || alive > max_alive) continue;
      drawfillbox(x, y, r);    
    }
  }
}
