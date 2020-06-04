#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define TOWER_SIZE 30
#define BLOCK_SIZE 100
#include "block.h"
#include "tower.h"


void moving_cubes_tower(float x, float y, float z, float red, float green, float blue, float animation_parameter, int leftRight) {
    
    glPushMatrix();
        glColor3f(red, green, blue);
        glTranslatef(x - leftRight*5*cos(animation_parameter/40.0), y, z);
        glScalef(1, 0.3/2, 1);
        glutSolidCube(1);
    glPopMatrix();
}

void draw_cube_tower(float x, float y, float z, float red, float green, float blue) {
    
    glPushMatrix();
        glColor3f(red, green, blue);
        glTranslatef(x, y, z);
        glScalef(1, 0.3/2, 1);
        glutSolidCube(1);
    glPopMatrix();
}


void setColorTower(int j) {
    
    if (!towers[j].cs) {
        
            if (towers[j].cf == 0) {
                towers[j].bl += 0.1;
            }
            
            if (towers[j].bl >= 0.7 && towers[j].cf == 0)
                towers[j].cf = 1;
            
            if (towers[j].cf == 1) {
                towers[j].gr -= 0.1;
            }
            
            if (towers[j].gr <= 0.0 && towers[j].cf == 1)
                towers[j].cf = 2;
            
            if (towers[j].cf == 2) {
                towers[j].rd += 0.1;
            }
            if (towers[j].rd >= 0.8 && towers[j].cf == 2) {
                towers[j].cf = 3;
                towers[j].gr = 0.1;
            }
            
            if (towers[j].cf == 3) {
                towers[j].bl -= 0.1;
            }
            if (towers[j].bl <= 0.2 && towers[j].cf == 3) {
                towers[j].cs = 1;
            }
        }
        else {
            
            if (towers[j].cs == 1)
                towers[j].bl += 0.1;
            
            if (towers[j].bl >= 0.7 && towers[j].cs == 1) {
                towers[j].cs = 2;
                towers[j].gr = 0.0;
            }
            
            if (towers[j].cs == 2)
                towers[j].rd -= 0.1;
            
            if (towers[j].rd <= 0.1 && towers[j].cs == 2)
                towers[j].cs = 3;
            
            if (towers[j].cs == 3)
                towers[j].gr += 0.1;
            
            if (towers[j].gr >= 0.7 && towers[j].cs == 3)
                towers[j].cs = 4;
            
            if (towers[j].cs == 4)
                towers[j].bl -= 0.1;
            
            if (towers[j].bl <= 0.4 && towers[j].cs == 4) {
                towers[j].cs = 0;
                towers[j].cf = 0;
                towers[j].rd = 0.1;
                towers[j].gr = 0.7;
                towers[j].bl = 0.5;
            }
        }
    
}


void initialize_tower_blocks() {
    
    towers[0].b[0].x = -15;
    towers[0].b[0].z = -1.7;
    towers[0].b[0].y = -3.0;
    towers[0].b[0].red = 0.1;
    towers[0].b[0].green = 0.7;
    towers[0].b[0].blue = 0.4;
    towers[0].param = 0;
    towers[0].h = 0.25 - 3.0;
    towers[0].lr = 1;

    towers[0].p = 1;
    towers[0].rd = 0.1;
    towers[0].gr = 0.7;
    towers[0].bl = 0.4;
    towers[0].cf = 0;
    towers[0].cs = 0;
    
    
    for (int j = 1;j < TOWER_SIZE;j ++) {
        
        
        if (j >= 1 && j < 10) {
        
            towers[j].b[0].x = towers[j-1].b[0].x + 2;
            towers[j].b[0].z = towers[0].b[0].z - 2;
        }
        else if (j >= 10 && j < 20) {
            towers[j].b[0].x = towers[j-10].b[0].x + 2.4;
            towers[j].b[0].z = towers[0].b[0].z - 4;
        }
        else {
            towers[j].b[0].x = towers[j-20].b[0].x + 2.8;
            towers[j].b[0].z = towers[0].b[0].z - 6;
        }
        
        towers[j].b[0].y = -3.0;
        towers[j].b[0].red = 0.1;
        towers[j].b[0].green = 0.7;
        towers[j].b[0].blue = 0.4;
        towers[j].param = 0;
        towers[j].h = 0.25 - 3.0;
        towers[j].lr = 1;
        towers[j].p = 1;
        towers[j].rd = 0.1;
        towers[j].gr = 0.7;
        towers[j].bl = 0.4;
        towers[j].cf = 0;
        towers[j].cs = 0;
    }
    
}
