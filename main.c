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



float animation_ongoing = 0;
float animation_parameter = 0.0;
//visina bloka
float h = 0.4;

void base_cube();
void moving_cubes();

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
        
    
    animation_parameter ++;
   
    
    
    glutPostRedisplay();

    if (animation_ongoing)
        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
}

void on_reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, (float) width/height, 1, 60);
}


void base_cube() {
    
    glColor3f(0.8, 0.2, 0.2);
    glScalef(2, 0.3, 2);
    glutSolidCube(1);
}

void moving_cubes() {
    
            
        glPushMatrix();
            glColor3f(0.6, 0.2, 0.2);
            glTranslatef(0 - 5*cos(animation_parameter/40.0), h, 0);
            glScalef(2, 0.3, 2);
            glutSolidCube(1);
        glPopMatrix();
        

}


void on_display() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    gluLookAt(7, 10, 10,
              0, 0, 0,
              0, 1, 0);
    


//       draw_axes(5);

    glPushMatrix();
        base_cube();
    glPopMatrix();
    

     
    glPushMatrix();
        moving_cubes();
    glPopMatrix();


    
    glutSwapBuffers();
}

    
    
    
    

