// Conways Game of Life cellular automata - sprite mode //

#include <SPI.h>
#include "GD.h"
#include "sphere.h"
#include "randreg.h"

#define GRID_X 16
#define GRID_Y 16

#define MAX_GENERATIONS 128

uint8_t grid[2][GRID_X][GRID_Y];
uint8_t current_grid = 0;
uint8_t generations = 0;


void setup(){

  GD.begin();

  GD.microcode(randreg_code, sizeof(randreg_code));
  srand(GD.rd(0x280E));

  GD.begin();

  GD.copy(RAM_SPRIMG, sphere_img, sizeof(sphere_img));
  GD.copy(RAM_SPRPAL, sphere_pal, sizeof(sphere_pal));
  
  GD.ascii();
  GD.putstr(15, 1, "Conways Game of Life");
  GD.putstr(16, 36, "Created by JLS 2021");
  
  for (int i = 0; i < 256; i++) GD.sprite(i, 400, 400, 0, 0, 0);
  
  initGrid();

}

void loop(){

  runGrid();
  drawGrid();
 
  if (generations > MAX_GENERATIONS) {
    generations = 0;
    initGrid();
  }

  generations++;

  delay(80);

  GD.waitvblank();
 
}


void initGrid(){

  current_grid = 0;
  
  for (int i = 0; i < GRID_X; i++) {
    for (int j = 0; j < GRID_Y; j++) {

      if (rand()%2) {
      grid[0][i][j] = 2; } else { grid[0][i][j] = 0; }
    }
  }
}

void runGrid(){
  
  uint8_t x, y;
  int count;
  uint8_t value = 0;
  uint8_t new_grid;

  new_grid = 1 - current_grid;
  
  for (y = 0; y < GRID_Y; y++) {
    for (x = 0; x < GRID_X; x++) {
      count = count_neighbours(x, y);
      if (count < 2 || count > 3) { value = 0; }
      else if (count == 3) { value = 3; }
      else { value = grid[current_grid][x][y]; }
      grid[new_grid][x][y] = value;
    }
  }
  current_grid = new_grid;
}

int count_neighbours(int x, int y){

  int sx;
  int result = 0;

  x--;
  y--;
  
  for (int i = 0; i < 3; i++) {
    if (y < 0 || y > (GRID_Y - 1)) { continue; }
    for (int j = 0; j < 3; j++) {
      if (x < 0 || x > (GRID_X - 1)) { continue; }
      if (i==1 && j == 1) { x++; continue; }
      if (grid[current_grid][x][y]) { result++; }
      x++;
    }
  y++;
  x -= 3;
  }
  return result;
}

void drawGrid(){
 
  uint8_t  x,  y;
  uint8_t cx, cy;
  uint8_t grid_next_colour = 0;
  
  cx = 0;
  cy = 0;
  for (y = 0; y < GRID_Y; y++) {
    cx = 0;
    for (x = 0; x < GRID_X; x++) {
      if (grid[1-current_grid][x][y] != grid[current_grid][x][y]) {
      if (grid[current_grid][x][y]) GD.sprite(x*y, 4*16+(16*cx), 16+(16*cy), 0, 0, 0);      
        else GD.sprite(x*y, 4*16+(16*cx), 16+(16*cy), 0, 1, 0);
      }
    cx += 1;
    }
    cy += 1;
  }

}
