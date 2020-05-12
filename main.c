#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

#define TIMER_INTERVAL 20
#define TIMER_ID 0
#define FILENAME0 "slike/pozadina.bmp"
#define FILENAME1 "slike/start.bmp"
#define FILENAME2 "slike/gameOver.bmp"
#define FILENAME3 "slike/win.bmp"

static GLuint texture[4];

static void on_display();
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int id);
static void initTexture();

struct Block {
    
    float y;
    float x;
    float red;
    float green;
    float blue;

};

struct Block blocks[100];


float animation_ongoing = 0;
float animation_parameter = 0.0;
//indikator za spustanje bloka
int drop = 0;
//visina bloka
float h = 0.4 - 3.0;
//brojac blokova
int i = 0;
int lr = 1;
float red = 0.1;
float green = 0.7;
float blue = 0.5;
int colorFirst = 0;
int colorSecond = 0;
float cameraEye = 0.0;
float cameraCenter = 0.0;
float eyeX = 7;
int fall = 1;
int n;
int k;
int end = 0;
int start = 1;
int gameOver = 0;
int win = 0;

void base_cube();
void moving_cubes(float y, float red, float green, float blue);
void draw_cube(float x, float y, float red, float green, float blue);
int nth_block();
void setColor();
void reset();
void drawScore();

int main(int argc, char **argv){
    /* Inicijalizacija */

    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    initTexture();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    float light_position[] = {-2, 3, 2, 1};
    float light_ambient[] = {.2f, .2f, .2f, 1};
    float light_diffuse[] = {.6f, .6f, .6f, 1};
    float light_specular[] = {.4f, .4f, .4f, 1};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);




    glClearColor(0.7, 0.7, 0.7, 0);
    glutMainLoop();

    return 0;
}

static void initTexture() {
    
    Image * image;
        /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    /*Inicijalizuje se objekat*/
    image = image_init(0, 0);

    /* Kreira se tekstura. */
    image_read(image, FILENAME0);

    /* Generisu se identifikatori tekstura. */
    glGenTextures(4, texture);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,  GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,  GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,image->width, image->height, 0,GL_RGB, GL_UNSIGNED_BYTE,image->pixels);
    
    image_read(image, FILENAME1);
    
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,  GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,  GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,image->width, image->height, 0,GL_RGB, GL_UNSIGNED_BYTE,image->pixels);
    
    image_read(image, FILENAME2);
    
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,  GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,  GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,image->width, image->height, 0,GL_RGB, GL_UNSIGNED_BYTE,image->pixels);
    
    image_read(image, FILENAME3);
    
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,  GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,  GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,image->width, image->height, 0,GL_RGB, GL_UNSIGNED_BYTE,image->pixels);

    /* Iskljucuje se tekstura */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat*/
    image_done(image);
}


/* Koordinatni sistem */
void draw_axes(float len) {
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(len,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,len,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,len);
    glEnd();

    glEnable(GL_LIGHTING);
}


void on_keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'd': // drop block 
        case 'D':
            drop = 1;
            break;
        case 's': // start animation
        case 'S':
            start = 0;
            if (!animation_ongoing) {
                animation_ongoing = 1;
                glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            }
            break;
        case 'r': // restart animation
        case 'R':
            reset();
            break;
        case 27:
          exit(0);
          break;
    }
}

void on_timer(int id) {
    if (id != TIMER_ID) 
        return;
        
    if (!end) {
    
        float pom;
        
        if (i < 5) {
                animation_parameter ++;
                pom = 0.002;
            }
            else if (i >= 5 && i <= 10) {
                animation_parameter += 1.4;
                pom = 0.003;
            }
            else if (i >= 11 && i < 20) {
                animation_parameter += 1.8;
                pom = 0.005;
            }
            else if (i >= 20 && i < 30){
                animation_parameter += 2.2;
                pom = 0.007;
            }
            else if (i >= 30 && i < 40){
                animation_parameter += 2.6;
                pom = 0.009;
            }
            else if (i >= 40 && i < 50) {
                animation_parameter += 3.0;
                pom = 0.011;
            }
            else if (i >= 50 && i <= 60){
                animation_parameter += 3.5;
                pom = 0.014;
            }
            else if (i >= 60 && i <= 70){
                animation_parameter += 3.9;
                pom = 0.016;
            }
            else if (i >= 70 && i <= 80){
                animation_parameter += 4.3;
                pom = 0.019;
            }
            else if (i >= 80 && i <= 90){
                animation_parameter += 4.5;
                pom = 0.022;
            }
            else {
                animation_parameter += 4.7;
                pom = 0.024;
            }
            
            if (i > 10 && i <= 40 && animation_parameter < 100) {
                cameraEye += 0.2;
                cameraEye += pom;
            }
            
            if (i > 40 && animation_parameter < 100) {
                cameraEye += 0.3;
                cameraEye += pom;
            }
    
            if (animation_parameter < 100)
                cameraCenter += pom;
        
        if (drop) {
            
            blocks[i].x = - lr*5*cos(animation_parameter/40.0);
            blocks[i].y = h - 0.1;
            blocks[i].red = red;
            blocks[i].green = green;
            blocks[i].blue = blue;
            
            if (fabs(blocks[i].x - blocks[i-1].x) > 2.0) {
                end = 1;
                n = i;
                k = n - 1;
                drop = 0;
            }
            else if (i == 99 && !(fabs(blocks[i].x - blocks[i-1].x) > 2.0)) {
                end = 1;
                n = 100;
                drop = 0;
            }

            lr *= -1;
            h += 0.3;
            animation_parameter = 0;
            i ++;
            setColor();
            
            
            if (i > 1) 
                cameraCenter += 0.05;
            
        }
    }
    else {
        
        if (n < 100 && fall) {
            
            if (k == -1) {
            
                if ((fabs(blocks[n].x) > 2.0)) {
                        blocks[n].y = -3.0;
                        fall = 0;
                }
                else {
                    blocks[n].y = blocks[0].y;
                    fall = 0;
                }
            }
            else {
            
                if ((fabs(blocks[n].x - blocks[k].x) > 2.0))
                            blocks[n].y -= 0.1;
                else {
                    blocks[n].y = blocks[k+1].y;
                    fall = 0;
                }
                
                
                if (blocks[n].y <= blocks[k+1].y)
                    k --;
            }

        }
        
        if (!fall) {
            
            if (!animation_parameter) {
            
                    if (eyeX > 2.0) {
                            eyeX -= 0.05;
                            if (i <= 40)
                                cameraEye -= 1.0;
                            else
                                cameraEye -= 0.6;
                    }
                    
                    else {
                            
                            
                        cameraCenter -= 0.05;
                        if (i <= 40)
                            cameraEye -= 0.6;
                        else
                            cameraEye -= 1.0;
                        
   
                        if (i <= 45 && cameraCenter <= 0.0) {
                            animation_parameter = 1;
                            
                        }
                        else if (i > 45 && cameraCenter <= 0.6){
                            animation_parameter = 1;
                            
                        }
                    
                    }
               }
               else {

                    if ( animation_parameter < 100)
                            animation_parameter ++;
                        
                    else {
                        
                        if (n == 100)
                            win = 1;
                                
                        else
                            gameOver = 1;
                                
                    }
               }
        }
    }
    
    glutPostRedisplay();

    if (animation_ongoing)
        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
}

void setColor() {
    
    if (!colorSecond) {
        
            if (colorFirst == 0) {
                blue += 0.1;
            }
            
            if (blue >= 0.7 && colorFirst == 0)
                colorFirst = 1;
            
            if (colorFirst == 1) {
                green -= 0.1;
            }
            
            if (green <= 0.0 && colorFirst == 1)
                colorFirst = 2;
            
            if (colorFirst == 2) {
                red += 0.1;
            }
            if (red >= 0.8 && colorFirst == 2) {
                colorFirst = 3;
                green = 0.1;
            }
            
            if (colorFirst == 3) {
                blue -= 0.1;
            }
            if (blue <= 0.2 && colorFirst == 3) {
                colorSecond = 1;
            }
        }
        else {
            
            if (colorSecond == 1)
                blue += 0.1;
            
            if (blue >= 0.7 && colorSecond == 1) {
                colorSecond = 2;
                green = 0.0;
            }
            
            if (colorSecond == 2)
                red -= 0.1;
            
            if (red <= 0.1 && colorSecond == 2)
                colorSecond = 3;
            
            if (colorSecond == 3)
                green += 0.1;
            
            if (green >= 0.7 && colorSecond == 3)
                colorSecond = 4;
            
            if (colorSecond == 4)
                blue -= 0.1;
            
            if (blue <= 0.4 && colorSecond == 4) {
                colorSecond = 0;
                colorFirst = 0;
                red = 0.1;
                green = 0.7;
                blue = 0.5;
            }
        }
    
}


void on_reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, (float) width/height, 1, 90);
}


void base_cube() {
    
    glColor3f(0.1, 0.7, 0.4);
    glTranslatef(- 0.8, -3.0, 0);
    glScalef(2, 0.3, 2);
    glutSolidCube(1);
}

void moving_cubes(float y, float red, float green, float blue) {
    
            
        glPushMatrix();
            glColor3f(red, green, blue);
            glTranslatef(-0.8 - lr*5*cos(animation_parameter/40.0), y, 0);
            glScalef(2, 0.3, 2);
            glutSolidCube(1);
        glPopMatrix();
        

}

void draw_cube(float x, float y, float red, float green, float blue) {
    
    glPushMatrix();
        glColor3f(red, green, blue);
        glTranslatef(x-0.8, y, 0);
        glScalef(2, 0.3, 2);
        glutSolidCube(1);
    glPopMatrix();
}





void on_display() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (start) {
         
        gluLookAt(0, 0, 5,
                  0, 0 , 0,
                  0, 1, 0);
    
    
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        
        glBegin(GL_QUADS);
        
            glNormal3f(0, 0, 1);

            glTexCoord2f(0, 0);
            glVertex3f(-3.45, -3.45,- 3);

            glTexCoord2f(1, 0);
            glVertex3f(3.45, -3.45, -3);

            glTexCoord2f(1, 1);
            glVertex3f(3.45, 3.45, -3);

            glTexCoord2f(0, 1);
            glVertex3f(-3.45, 3.45, -3);
        
        glEnd();
        
    
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else if (win) {
        gluLookAt(0, 0, 5,
                  0, 0 , 0,
                  0, 1, 0);
    
    
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        
        glBegin(GL_QUADS);
        
            glNormal3f(0, 0, 1);

            glTexCoord2f(0, 0);
            glVertex3f(-3.45, -3.45,- 3);

            glTexCoord2f(1, 0);
            glVertex3f(3.45, -3.45, -3);

            glTexCoord2f(1, 1);
            glVertex3f(3.45, 3.45, -3);

            glTexCoord2f(0, 1);
            glVertex3f(-3.45, 3.45, -3);
        glEnd();
            
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else if (gameOver) {
            
        gluLookAt(0, 0, 5,
                  0, 0 , 0,
                  0, 1, 0);
    
    
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        
        glBegin(GL_QUADS);
        
            glNormal3f(0, 0, 1);

            glTexCoord2f(0, 0);
            glVertex3f(-3.45, -3.45,- 3);

            glTexCoord2f(1, 0);
            glVertex3f(3.45, -3.45, -3);

            glTexCoord2f(1, 1);
            glVertex3f(3.45, 3.45, -3);

            glTexCoord2f(0, 1);
            glVertex3f(-3.45, 3.45, -3);
        
        glEnd();
            
        glBindTexture(GL_TEXTURE_2D, 0);

    }
    else {
    
        gluLookAt(0, 0, 16,
                  0, 0 , 0,
                  0, 1, 0);
            
            
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        
        glBegin(GL_QUADS);
        
            glNormal3f(0, 0, 1);

            glTexCoord2f(0, 0);
            glVertex3f(-9.15, -9.15,- 6);

            glTexCoord2f(1, 0);
            glVertex3f(9.15, -9.15, -6);

            glTexCoord2f(1, 1);
            glVertex3f(9.15, 9.15, -6);

            glTexCoord2f(0, 1);
            glVertex3f(-9.15, 9.15, -6);
        
        glEnd();
            
        
        glBindTexture(GL_TEXTURE_2D, 0);
        
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        

        // 7 10 10
        //2 5 10
        gluLookAt(eyeX, 5 + cameraEye/20.0, 10,
                0, 0 + cameraCenter, 0,
                0, 1, 0);
        


    //       draw_axes(5);

        glPushMatrix();
            base_cube();
        glPopMatrix();
        

        if (!end) {
        
            glPushMatrix();
                moving_cubes(h, red, green, blue);
            glPopMatrix();
        }
        
        if (end && n < 100)
            draw_cube(blocks[n].x, blocks[n].y, blocks[n].red, blocks[n].green, blocks[n].blue);
        
            
        for (int j = 0;j < i;j ++) {

            glPushMatrix();
            draw_cube(blocks[j].x, blocks[j].y, blocks[j].red, blocks[j].green, blocks[j].blue);
            glPopMatrix();
        
            drop = 0;
        }
        
        if(end && animation_parameter)
                drawScore();
    }
    
    glutSwapBuffers();
}
    
void reset() {
    
    animation_ongoing = 0;
    animation_parameter = 0;
    drop = 0;
    h = 0.4 - 3.0;
    lr = 1;
    cameraCenter = 0;
    cameraEye = 0;
    end = 0;
    
    for (int j = 0;j < 100;j ++) {
        blocks[j].x = 0.0;
        blocks[j].y = 0.0;
        blocks[j].red = 0.0;
        blocks[j].green = 0.0;
        blocks[j].blue = 0.0;
    }
    
    i = 0;
    red = 0.1;
    green = 0.7;
    blue = 0.5;
    colorFirst = 0;
    colorSecond = 0;
    n = 500;
    gameOver = 0;
    start = 1;
    win = 0;
    eyeX = 7;
    fall = 1;
}

void drawScore() {
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  
    gluLookAt(0, 0, -9,
              0, 0, 0,
              0, 1, 0);

    char s[15];
    int len = 0;
    
    if (n == 100)
        sprintf(s, "SCORE : %d", i);
    else
        sprintf(s, "SCORE : %d", i-1);
    len = strlen(s);
    
    glColor3f(0.7, 0.7, 0.7);
    glRasterPos2f(3.3, -3.45);

    for (int j = 0;j < len;j ++) 
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[j]);
    
    
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        
        glVertex2f(2.0, -3.2);
        glVertex2f(3.6, -3.2);
        glVertex2f(3.6, -3.6);
        glVertex2f(2.0, -3.6);
        
    glEnd();
}    

