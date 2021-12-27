// Conways Game of Life cellular automata //

#include "SPI.h"
#include "GD.h"
#include "randreg.h"

#define CONST 9
#define WIDTH 256/CONST
#define HEIGHT 256/CONST
#define MAX_GEN 250

void RandGrid();
void RunGrid();
void DrawGrid();
int Neighbours(int x, int y);

  byte grid[2][WIDTH][HEIGHT];
  int current, generations;
  char msg1[30], msg2[30];
  int i, j;
   
byte replicate(byte color)
{
  return (color << 6) | (color << 4) | (color << 2) | color;
}
           
void setpixel(byte x, byte y, byte color)
{
  unsigned int addr = RAM_SPRIMG|(x & 0xf)|(y << 4)|((x & 0x30)<<8);
  byte mask = 0xc0 >> ((x >> 5) & 6);
  GD.wr(addr, (GD.rd(addr) & ~mask) | (color & mask));
}

void drawfillbox(byte x, byte y, byte color)
{
  int nx ,ny;
  for (ny = 0; ny < CONST-2; ny++) {
    for (nx = 0; nx < CONST-2; nx++) setpixel (2+(CONST*x)+nx, 2+(CONST*y)+ny, replicate((color+1) & 3));
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
  GD.putstr(6, 1, "Conways Game of Life cellular automata");
  GD.putstr(16, 36, "Created by JLS 2021");
    
  RandGrid();
    
}


void loop(){ 
    
  DrawGrid();
  RunGrid();

  generations++;

  GD.putstr(1, 36, msg1);
  sprintf(msg1, "%3d cnt", GD.rd(0x2802));

  GD.putstr(41, 36, msg2);
  sprintf(msg2, "%3d gens", generations);
  
  if (generations > MAX_GEN) {
    generations = 0;
    RandGrid();
  }

  GD.waitvblank();
    
}

void RandGrid(){
  
  GD.fill(RAM_SPRIMG, 0, 16384);
  current = 0;
  generations = 0;
  for (i = 0; i < WIDTH; i++) {
    for (j = 0; j < HEIGHT; j++) {
      grid[0][i][j] =  GD.rd(0x280E)%2;
      grid[1][i][j] = 0;
    }
  }
}

void RunGrid(){

  int count;
  int value = 0;
  int new_grid;

  new_grid = 1 - current;
  
  for (int y = 0; y < HEIGHT; y++) {
  
    for (int x = 0; x < WIDTH; x++) {
      
      count = Neighbours(x, y);
      
      if (count < 2 || count > 3) { value = 0; }
      else if (count == 3) { value = 3; }
      else { value = grid[current][x][y]; }
    
      grid[new_grid][x][y] = value;
    
    }
  }
  
  current = new_grid;
}


int Neighbours(int x, int y){

  int result = 0;

  x--;
  y--;
  
  for (int i = 0; i < 3; i++) {
  
    if (y < 0 || y > (HEIGHT - 1)) continue;

    for (int j = 0; j < 3; j++) {
      if (x < 0 || x > (WIDTH - 1)) continue;
      if (i==1 && j == 1) { x++; continue; }
      if (grid[current][x][y]) result++;
      x++;
    }
    y++;
    x -= 3;
  }
  
  return result;
} 


void DrawGrid(){
  
  int cx, cy;
  byte col;
  cx = 0;
  cy = 0;
  
  for (int y = 0; y < HEIGHT; y++) {
    cx = 0;
    for (int x = 0; x < WIDTH; x++) {
      if (grid[1-current][x][y] != grid[current][x][y]) {
        if(grid[current][x][y]) col = 0;
        else col = 255;
        drawfillbox(cx, cy, col);
        }
    cx += 1;
    }
    cy += 1;
  }
}
