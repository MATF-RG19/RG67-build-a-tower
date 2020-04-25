#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define TIMER_INTERVAL 20
#define TIMER_ID 0


static void on_display();
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int id);

struct Block {
    
    float y;
    float x;
    float color;

};

struct Block* blocks;


float animation_ongoing = 0;
float animation_parameter = 0.0;
//indikator za spustanje bloka
int drop = 0;
//visina bloka
float h = 0.4 - 3.0;
//brojac blokova
int i = 0;
//indikator da li blok dolazi sleva ili sdesna
int lr = 1;
float color = 0.2;
int mem = 50;
float cameraEye = 0.0;
float cameraCenter = 0.0;

void base_cube();
void moving_cubes(float y, float color);
void draw_cube(float x, float y, float color);
void checkMem();

int main(int argc, char **argv){
    /* Inicijalizacija */
    

    blocks = malloc(mem * sizeof(struct Block));
    if (blocks == NULL) {
        fprintf(stderr, "Greska u alokaciji memorije\n");
        exit(1);
    }

    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(900, 900);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);

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
            if (!animation_ongoing) {
                animation_ongoing = 1;
                glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            }
            break;
        case 27:
          exit(0);
          break;
    }
}

void on_timer(int id) {
    if (id != TIMER_ID) 
        return;
        
    float pom;
    
    if (i < 5) {
            animation_parameter ++;
            pom = 0.002;
        }
        else if (i >= 5 && i <= 10) {
            animation_parameter += 1.5;
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
        
        checkMem();
        blocks[i].x = - lr*5*cos(animation_parameter/40.0);
        blocks[i].y = h - 0.1;
        blocks[i].color = color;
        
        lr *= -1;
        h += 0.3;
        if (color >= 1.0)
            color = 0.0;
        color += 0.1;
        animation_parameter = 0;
        i ++;
         if (i > 1) 
            cameraCenter += 0.05;
        
    }
    
    glutPostRedisplay();

    if (animation_ongoing)
        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
}

void on_reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, (float) width/height, 1, 90);
}


void base_cube() {
    
    glColor3f(0.8, 0.2, 0.2);
    glTranslatef(- 0.8, -3.0, 0);
    glScalef(2, 0.3, 2);
    glutSolidCube(1);
}

void moving_cubes(float y, float color) {
    
            
        glPushMatrix();
            glColor3f(0.6, color, 0.2);
            glTranslatef(-0.8 - lr*5*cos(animation_parameter/40.0), y, 0);
            glScalef(2, 0.3, 2);
            glutSolidCube(1);
        glPopMatrix();
        

}

void draw_cube(float x, float y, float color) {
    
    glPushMatrix();
        glColor3f(0.6, color, 0.2);
        glTranslatef(x-0.8, y, 0);
        glScalef(2, 0.3, 2);
        glutSolidCube(1);
    glPopMatrix();
}





void on_display() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 7 10 10
    //2 5 10
    gluLookAt(7, 5 + cameraEye/20.0, 10,
              0, 0 + cameraCenter, 0,
              0, 1, 0);
    


//       draw_axes(5);

    glPushMatrix();
        base_cube();
    glPopMatrix();
    

     
    glPushMatrix();
        moving_cubes(h, color);
    glPopMatrix();
    
    
        
        for (int j = 0;j < i;j ++) {

        glPushMatrix();
            draw_cube(blocks[j].x, blocks[j].y, blocks[j].color);
        glPopMatrix();
        
         drop = 0;
    }

    
    
    glutSwapBuffers();
}
    
void checkMem(){

    if(i == mem) {

        mem = mem * 2;
        blocks = realloc(blocks, mem * sizeof(struct Block));
        
        if (blocks == NULL) {
            printf("Neuspela realokacija\n");
            exit(1);
        }
    }
}
