// Lorenz chaotic attractor - sprite mode //

#include <SPI.h>
#include "GD.h"
#include "sphere.h"

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
       
  int cnt;


void setup(){
  
  int i;

  GD.begin();

  GD.copy(RAM_SPRIMG, sphere_img, sizeof(sphere_img));
  GD.copy(RAM_SPRPAL, sphere_pal, sizeof(sphere_pal));
  
  GD.ascii();
  GD.putstr(14, 1, "Lorenz Chaotic Attractor");
  GD.putstr(16, 36, "Created by JLS 2021");
  
  for (i = 0; i < 256; i++) GD.sprite(i, 400, 400, 0, 0, 0);

}

void loop(){
        
  for (int i = 0; i < 256; i++) {
        
    xn = s*(y-x);
    yn = (r*x)-y-(x*z);
    zn = (x*y)-(b*z);

    x = x+xn*dt;
    y = y+yn*dt;
    z = z+zn*dt;
        
    if (cnt == 4) {

      GD.sprite(i, 200+(9*x), 150+(5*y), 0, 0, 0);
      cnt = 0;
       
     }
       
    cnt ++;
       
  }

  GD.waitvblank();

}
