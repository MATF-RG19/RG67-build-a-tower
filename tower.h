#define TOWER_SIZE 30
#define BLOCK_SIZE 100
#define TOWER_BLOCK_SIZE 300

//struktura za tower
struct Tower {
    struct Block b[TOWER_BLOCK_SIZE];
    float h;
    int lr;
    float param;
    int p;
    float rd;
    float gr;
    float bl;
    int cf;
    int cs;
};


struct Tower towers[TOWER_SIZE];

// crta blok dok se pomera
void moving_cubes_tower(float x, float y, float z, float red, float green, float blue, float animation_parameter, int leftRight);
// crta vec postavljene blokove
void draw_cube_tower(float x, float y, float z, float red, float green, float blue);
//postavlja boju bloka za j-ti tower
void setColorTower(int j);
//inicjalizuje niz towers
void initialize_tower_blocks(); 
