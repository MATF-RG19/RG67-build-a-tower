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


//struktura koja cuva x, y koordinate i rgb bloka
struct Block {
    
    float y;
    float x;
    float red;
    float green;
    float blue;

};

/* PROMENLJIVE */

// identifikatori tekstura
static GLuint texture[4];

// niz blokova
struct Block blocks[100];

// promenljive za timer funkciju
float animation_ongoing = 0;
float animation_parameter = 0.0;

//indikator za spustanje bloka
int drop = 0;
//visina bloka
float h = 0.4 - 3.0;
//brojac blokova
int i = 0;
//indikator da li blok dolazi sleva ili zdesna
int leftRight = 1;

//parametri za pomeranje kamere, cameraEye - EyeY cameraCenter - CenterY u gluLookAt funkciji
float cameraEye = 0.0;
float cameraCenter = 0.0;

// pocetna vrednost za koju pomeramo eyex na kraju kada prikazujemo celu kulu
float eyeX = 7;

//rgb za naredni blok
float red = 0.1;
float green = 0.7;
float blue = 0.5;
//odredjuju smer u kome se boje blokova menjaju
int colorFirst = 0;
int colorSecond = 0;


// indikator za kraj igre
int end = 0;
// indikator za startni prozor
int start = 1;
// indikator za gameOver prozor
int gameOver = 0;
// indikator za win prozor
int win = 0;

// indikator za spustanje poslednjeg bloka
int fall = 1;
// promenjiva u kojoj cemo cuvati indeks poslednjeg bloka
int n;
int k;

/* FUNKCIJE */

static void on_display();
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int id);
// Inicijalizacija teksture
static void initTexture();
// vraca promenljive na pocetno stanje
void reset();
// crta pocetni blok na koji treba da slazemo ostale
void base_cube();
// crta blok dok se pomera
void moving_cubes(float y, float red, float green, float blue);
// crta vec postavljene blokove
void draw_cube(float x, float y, float red, float green, float blue);
// postavlja boju sledeceg bloka
void setColor();
// ispisuje rezultat - broj postavljenih blokova
void drawScore();



int main(int argc, char **argv){
   
    // inicijalizuje se glut 

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    // kreira se prozor
    
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    // callback funkcije
    
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);

    glClearColor(0.7, 0.7, 0.7, 0);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    // inicijalizuju se teksture
    initTexture();
    
    // postavlja se osvetljenje
    
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
    
    glutMainLoop();

    return 0;
}

static void initTexture() {
    
    /*Inicijalizuje se objekat*/
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
        case 27: // quit
          exit(0);
          break;
    }
}

void on_timer(int id) {
    
    if (id != TIMER_ID) 
        return;
        
    
    if (!end) {
    
        // pomocni parametar za uvecavanje cameraCenter i cameraEye
        float pom;
    
        // uvecavamo animation_parameter, cameraCenter i cameraEye
        // sto vise blokova ima to brze treba da se krecu
        
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
            pom = 0.023;
        }
        
        if (i > 10 && i <= 40 && animation_parameter < 100) {
            cameraEye += 0.2;
            cameraEye += pom;
        }
        
        if (i > 40 && i < 90 && animation_parameter < 100) {
            cameraEye += 0.3;
            cameraEye += pom;
        }
        if (i >= 90 && animation_parameter < 100) {
            cameraEye += 0.5;
            cameraEye += pom;
        }
   
        if (animation_parameter < 100)
            cameraCenter += pom;


        
        // kada pritisnemo d zapisujemo i-ti blok u niz blocks
        if (drop) {
            
            blocks[i].x = - leftRight*5*cos(animation_parameter/40.0);
            blocks[i].y = h - 0.1;
            blocks[i].red = red;
            blocks[i].green = green;
            blocks[i].blue = blue;
            
            
            // ako se ne poklapa sa prethodnim zavrsili smo igricu
            if (fabs(blocks[i].x - blocks[i-1].x) > 2.0) {
                //pamtimo koji blok je bio poslednji kako bismo ga spustili kasnije
                n = i;
                k = n - 1;
                // zavrsio se prvi deo
                end = 1;
                    
                }
            // ako je i = 99 znaci da smo na stotom bloku 
            //i ako smo ga pravilno spustili pobedili smo    
            else if (i == 99 && !(fabs(blocks[i].x - blocks[i-1].x) > 2.0)) {
                // postavljamo n na 100 da znamo da smo pobedili
                n = 100;
                // zavrsio se prvi deo
                end = 1;
                // pobedili smo pa fall postavljamo na 0
                // jer je poslenji blok vec na svom mestu
                fall = 0;
            }
            //postavljamo parametre za sledeci blok
            
                //uvecavamo broj blokova
                i ++;
            
                // menjamo stranu sa koje dolazi sledeci blok
                leftRight *= -1;
                // uvecavamo visinu sledeceg bloka
                h += 0.3;
                // vracamo animation_parameter na pocetnu vrednost
                animation_parameter = 0;
                
                //postavljamo boju sledeceg bloka  
                setColor();
                
                // pomeramo cameraCenter svaki put kada spustimo blok
                if (i > 1) 
                    cameraCenter += 0.05;
            
            
            
        }
    
    }
    
    //drugi deo, kada korisnik zavrsi sa igricom (end = 1)
    
    else {
 
        // ako nismo pobedili tj. n je < 100
        // spustamo n-ti (poslednji) blok koji se nije poklopio sa prethodnim
        // treba da nalegne na prvi blok sa kojim se poklapa
        // ili da padne do kraja ako se ne poklapa ni sa jednim 
        // kada ga smestimo na odgovarajucu poziciju fall postavljamo na 0
        
        
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
        
        
        // kada je n-ti blok pao na svoje mesto 
        // pomeramo kameru ka dole kako bismo videli celu kulu

           if (!fall) {
               
               if (!animation_parameter) {
               
                    //prvo pomeramo xEye i yEye gluLookAt funkcije dok xEye ne dodje do 2
                    if (eyeX > 2.0) {
                            eyeX -= 0.05;
                            if (i <= 40)
                                cameraEye -= 1.0;
                            else
                                cameraEye -= 0.6;
                    }
                    
                    // pomeramo kameru dole dok ne stignemo do kraja
                    else {
                            
                            
                        cameraCenter -= 0.05;
                        if (i <= 40)
                            cameraEye -= 0.6;
                        else
                            cameraEye -= 1.0;
                        
                        //kada dodjemo do podloge kule stajemo

                        if (i <= 15 && cameraCenter <= 0.2) {
                            animation_parameter = 1;
                            
                        }
                        else if (i > 15 && cameraCenter <= 0.6){
                            animation_parameter = 1;
                            
                        }
                    
                    }
               }
               else {
                   
                   // zadrzavamo se na kraju na kratko da bismo videli score
                   if ( animation_parameter < 200)
                        animation_parameter ++;
                   // prelazimo na naredni prozor
                    else {
                        // ako smo pobedili n je 100
                        // i prikazujemo win prozor
                        if (n == 100)
                            win = 1;
                        // inace smo izgubili
                        // i pokazujemo gameOver prozor
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



void on_display() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    // pocetni prozor koji stoji dok  ne pritisnemo 's'
    if (start) {
        
        
        // postavljamo sliku za startni prozor 
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
   
    // na kraju postavljamo win prozor ako smo pobedili
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
    // na kraju postavljamo gameOver prozor ako smo izgubili
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

        // postavljanje pozadine
    
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
        
        // dok se blokovi slazu pomeramo kameru ka gore - kada je end = 0
        // a kada je end = 1 prikazujemo celu kulu
        gluLookAt(eyeX, 5 + cameraEye/20.0, 10,
                    0, 0 + cameraCenter, 0,
                    0, 1, 0);

        // postavljanje prvog bloka 
        glPushMatrix();
            base_cube();
        glPopMatrix();
        
        if (!end) {    
            // blok koji treba spustiti na prethodni
            glPushMatrix();
                moving_cubes(h, red, green, blue);
            glPopMatrix();
        }
        
        // ako je end = 1 i nismo pobedili crtamo poslednji blok koji se nije poklopio
        if (end && n < 100)
            draw_cube(blocks[n].x, blocks[n].y, blocks[n].red, blocks[n].green, blocks[n].blue);
            
        // crtamo vec postavljene blokove koje smo zapamtili u nizu blocks    
        for (int j = 0;j < i;j ++) {

            glPushMatrix();
                draw_cube(blocks[j].x, blocks[j].y, blocks[j].red, blocks[j].green, blocks[j].blue);
            glPopMatrix();

            
        }
        // vracamo drop na 0 kako bismo mogli da spustimo naredni blok
        drop = 0;
        
        // ispisujemo score na kraju
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
    leftRight = 1;
    cameraCenter = 0;
    cameraEye = 0;
    
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
    eyeX = 7;
    fall = 1;
    n = 500;
    start = 1;
    end = 0;
    win = 0;
    gameOver = 0;
}
    
    
void drawScore() {
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  
    gluLookAt(0, 0, -9,
              0, 0, 0,
              0, 1, 0);

    //niska u koju ispisujemo score
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
    
    
    //okvir za score
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        
        glVertex2f(2.0, -3.2);
        glVertex2f(3.6, -3.2);
        glVertex2f(3.6, -3.6);
        glVertex2f(2.0, -3.6);
        
    glEnd();
}    
