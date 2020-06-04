#define TOWER_SIZE 30
#define BLOCK_SIZE 100

//struktura koja cuva x, y koordinate i rgb bloka
struct Block {
    
    float x;
    float y;
    float z;
    float red;
    float green;
    float blue;

};

// crta pocetni blok na koji treba da slazemo ostale
void base_cube();
// crta blok dok se pomera
void moving_cubes(float y, float red, float green, float blue, float animation_parameter, int leftRight);
// crta vec postavljene blokove
void draw_cube(float x, float y, float red, float green, float blue);



 




