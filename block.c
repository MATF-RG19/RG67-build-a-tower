#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define TOWER_SIZE 30
#define BLOCK_SIZE 100
#include "block.h"

void base_cube() {
    glPushMatrix();
        glColor3f(0.1, 0.7, 0.4);
        glTranslatef(- 0.8, -3.0, 0);
        glScalef(2, 0.3, 2);
        glutSolidCube(1);
    glPopMatrix();
}

void moving_cubes(float y, float red, float green, float blue, float animation_parameter, int leftRight) {
    
            
    glPushMatrix();
        glColor3f(red, green, blue);
        glTranslatef(- 0.8 - leftRight*5*cos(animation_parameter/40.0), y, 0);
        glScalef(2, 0.3, 2);
        glutSolidCube(1);
    glPopMatrix();
        

}


void draw_cube(float x, float y, float red, float green, float blue) {
    
    glPushMatrix();
        glColor3f(red, green, blue);
        glTranslatef(x- 0.8, y, 0);
        glScalef(2, 0.3, 2);
        glutSolidCube(1);
    glPopMatrix();
}





















