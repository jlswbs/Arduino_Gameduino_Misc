// Lorenz chaotic attractor - sprite mode with background //

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
  GD.ascii();
  
   for (byte y = 0; y < 38; y++) {
    const unsigned char *src = ramp_pic + y * 4;
    for (byte x = 0; x < 50; x++) GD.wr(RAM_PIC + y * 64 + x, pgm_read_byte(src));
  }
  
  GD.copy(RAM_CHR + 128 * 16, ramp_chr, sizeof(ramp_chr));
  GD.copy(RAM_PAL + 128 * 8, ramp_pal, sizeof(ramp_pal));
  
  GD.copy(RAM_SPRIMG, sphere_img, sizeof(sphere_img));
  GD.copy(RAM_SPRPAL, sphere_pal, sizeof(sphere_pal));

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
