// Rubik Cube in OpenGL
/* TODO: INSERIRE CONTORNI NERI SU FACCE */
/* TODO: ELIMINARE GAP TRA QUADRATI */
/* TODO: RINOMINARE TUTTE LE FUNZIONI */
/* TODO: EVIDENZIARE LA RIGA/COLONNA CHE SI STA PER MUOVERE */
/* TODO: TROVARE UN RIMPIAZZO A vector OPPURE CAPIRE COSA FA DI PRECISO */
/* TODO: METTERE UNA TEXTURE DI SFONDO PER RENDERLO PIU' CARINO */
/* TODO: CAPIRE A CHE CAZZO SERVE using namespace std; */
/* TODO: AGGIUNGERE UN MENU CHE SI ATTIVA CON TASTO DESTRO PER CAMBIARE SFONDO? */



#include "readBMP.h"
#include "data_path.h"

#define NFACES  6

using namespace std;

struct cube_rotate {
    GLfloat angle, x, y, z;
};


static struct BitMapFile *images[NFACES];




// parameters to bind the textures
GLenum facePos[NFACES] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                          GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                          GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};


GLfloat angle, fAspect, cube_size;
GLint rot_x, rot_y, crement, x_0, x_k, y_0, y_k, z_0, z_k, gap, gap_crement;
//cube_rotate cube_rotations[3][3][3];
vector<cube_rotate> cube_rotations[3][3][3];

/* L'ho commentata perche' sembra non servire... scritta cosi' e' solo un prototipo */
// void load_visualization_parameters(void);

void apply_rotation(GLfloat angle) {

    vector<cube_rotate> face[3][3];
    int index;
    cube_rotate rotation;

    // copy face to be rotated
    // apply rotation to face
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {

            index = 2 - j % 3;

            if (x_0 == x_k) {
                rotation = {angle, 1.0, 0.0, 0.0};
                face[index][i] = cube_rotations[x_k][i][j];
            }

            if (y_0 == y_k) {
                rotation = {angle, 0.0, 1.0, 0.0};
                face[index][i] = cube_rotations[j][y_k][i];
            }

            if (z_0 == z_k) {
                rotation = {-1 * angle, 0.0, 0.0, 1.0};
                face[index][i] = cube_rotations[j][i][z_k];
            }

            face[index][i].push_back(rotation);

        }

    // copy back rotated face
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {

            if (x_0 == x_k)
                cube_rotations[x_k][i][j] = face[i][j];

            if (y_0 == y_k)
                cube_rotations[j][y_k][i] = face[i][j];

            if (z_0 == z_k)
                cube_rotations[j][i][z_k] = face[i][j];
        }

}

// reset face selection parameters
void reset_selected_face() {

    x_0 = 0;
    x_k = 2;
    y_0 = 0;
    y_k = 2;
    z_0 = 0;
    z_k = 2;

}

void set_camera() {
    gluLookAt(0, 80, 200, 0, 0, 0, 0, 1, 0);
}

// draw a cube
void draw_cube(int x, int y, int z) {

    vector<cube_rotate> lrot = cube_rotations[x][y][z];

    glPushMatrix();

    // translate to final position
    glTranslatef((x - 1) * cube_size + x * gap, (y - 1) * cube_size + y * gap, (z - 1) * cube_size + z * gap);

    // rotate cube to correct position
    for (int i = lrot.size() - 1; i >= 0; --i)
        glRotatef(lrot[i].angle, lrot[i].x, lrot[i].y, lrot[i].z);

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);  // front
    glNormal3f(0.0, 0.0, 1.0);  // face normal
    glVertex3f(cube_size / 2, cube_size / 2, cube_size / 2);
    glVertex3f(-cube_size / 2, cube_size / 2, cube_size / 2);
    glVertex3f(-cube_size / 2, -cube_size / 2, cube_size / 2);
    glVertex3f(cube_size / 2, -cube_size / 2, cube_size / 2);
    glEnd();

    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(cube_size / 2, cube_size / 2, -cube_size / 2);
    glVertex3f(cube_size / 2, -cube_size / 2, -cube_size / 2);
    glVertex3f(-cube_size / 2, -cube_size / 2, -cube_size / 2);
    glVertex3f(-cube_size / 2, cube_size / 2, -cube_size / 2);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);  // left
    glNormal3f(-1.0, 0.0, 0.0);  // face normal
    glVertex3f(-cube_size / 2, cube_size / 2, cube_size / 2);
    glVertex3f(-cube_size / 2, cube_size / 2, -cube_size / 2);
    glVertex3f(-cube_size / 2, -cube_size / 2, -cube_size / 2);
    glVertex3f(-cube_size / 2, -cube_size / 2, cube_size / 2);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);  // right
    glNormal3f(1.0, 0.0, 0.0);  // face normal
    glVertex3f(cube_size / 2, cube_size / 2, cube_size / 2);
    glVertex3f(cube_size / 2, -cube_size / 2, cube_size / 2);
    glVertex3f(cube_size / 2, -cube_size / 2, -cube_size / 2);
    glVertex3f(cube_size / 2, cube_size / 2, -cube_size / 2);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);  // top
    glNormal3f(0.0, 1.0, 0.0);  // face normal
    glVertex3f(-cube_size / 2, cube_size / 2, -cube_size / 2);
    glVertex3f(-cube_size / 2, cube_size / 2, cube_size / 2);
    glVertex3f(cube_size / 2, cube_size / 2, cube_size / 2);
    glVertex3f(cube_size / 2, cube_size / 2, -cube_size / 2);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);  // bottom
    glNormal3f(0.0, -1.0, 0.0);  // face normal
    glVertex3f(-cube_size / 2, -cube_size / 2, -cube_size / 2);
    glVertex3f(cube_size / 2, -cube_size / 2, -cube_size / 2);
    glVertex3f(cube_size / 2, -cube_size / 2, cube_size / 2);
    glVertex3f(-cube_size / 2, -cube_size / 2, cube_size / 2);
    glEnd();

    glPopMatrix();

} // draw cube function

// draw function
void display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // reset transformations
    glLoadIdentity();

    // set camera position
    set_camera();

    // apply visualization transformations
    glRotatef(rot_x, 1.0, 0.0, 0.0); // rotate in y axis
    glRotatef(rot_y, 0.0, 1.0, 0.0); // rotate in x axis

    for (int i = 0; i < 3; ++i) // step through x axis
        for (int j = 0; j < 3; ++j) // step through y axis
            for (int k = 0; k < 3; ++k) { // step through z axis

                // draw a single cube
                draw_cube(i, j, k);
                /* Messo per capire quali sono le coordinate dei vari cubi */
                printf("i %d , j %d, k %d\n", i, j, k);

            }

    // flush opengl commands
    glutSwapBuffers();

}

// init rendering parameters
void init(void) {

    // init parameters
    cube_size = 30.0; // cuboid size
    rot_x = 0.0; // view rotation x
    rot_y = 0.0; // view rotation y
    crement = 5; // rotation (in/de)crement
    gap = 1;
    gap_crement = 3;
    // initialize cuboid rotations

    // init lighting
    GLfloat ambient_lighte[4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat diffuse_light[4] = {0.7, 0.7, 0.7, 1.0};        // color
    GLfloat specular_light[4] = {1.0, 1.0, 1.0, 1.0};    // brightness
    GLfloat light_position[4] = {0.0, 50.0, 50.0, 1.0};

    // material brightness capacity
    GLfloat specularity[4] = {1.0, 1.0, 1.0, 1.0};
    GLint material_specularity = 60;

    // black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Gouraud colorization model
    glShadeModel(GL_SMOOTH);

    // material reflectability
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularity);
    // brightness concentration
    glMateriali(GL_FRONT, GL_SHININESS, material_specularity);

    // activate ambient light
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_lighte);

    // define light parameters
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_lighte);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // enable changing material color
    glEnable(GL_COLOR_MATERIAL);
    // enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // enable depth buffering
    glEnable(GL_DEPTH_TEST);

    angle = 45;




    /* PER LEGGERE IMMAGINI */
    /*
     *
     *
     *
     *
     *
     *
     * */

    // local variables
    int currInd;

    for(currInd=0; currInd<NFACES; currInd++) {
        // Load external textures
        images[currInd] = readBMP(fileName[currInd]);
        if(images[currInd] == NULL) {
            printf("Could not open file %s\n", fileName[currInd]);
        } else {
            printf("Loaded image %s - %d x %d pixels \n", fileName[currInd],
                   images[currInd]->sizeX, images[currInd]->sizeY);
        }

        // generate textures
        glTexImage2D(facePos[currInd], 0, GL_RGBA, images[currInd]->sizeX,
                     images[currInd]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, images[currInd]->data);

    }



    /* FINE LEGGERE IMMAGINI */
    /*
     *
     *
     *
     *
     *
     *
     * */






} // init

// specify what's shown in the window
void load_visualization_parameters(void) {
    // specify projection coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // specify projection perspective
    gluPerspective(angle, fAspect, 0.4, 500);

    // init model coordinate system
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // specify observer and target positions
    set_camera();
} // load visualization parameters

// window reshape callback
void reshape_func(GLsizei w, GLsizei h) {
    // prevents division by zero
    if (h == 0) h = 1;

    // viewport size
    glViewport(0, 0, w, h);

    // aspect ratio
    fAspect = (GLfloat) w / (GLfloat) h;

    load_visualization_parameters();
} // reshape function

// keyboard function callback
void keyboard_func(unsigned char key, int x, int y) {

    switch (key) {

        case '+':
            if (angle >= 10) angle -= 5;
            load_visualization_parameters();
            break;

        case '-':
            if (angle <= 130) angle += 5;
            load_visualization_parameters();
            break;
            // vielw rotation
            // INcrement or DEcrement
        case 'L': // right
        case 'l':
            rot_y = (rot_y - crement) % 360;
            break;

        case 'J': // left
        case 'j':
            rot_y = (rot_y + crement) % 360;
            break;

        case 'I': // down
        case 'i':
            rot_x = (rot_x + crement) % 360;
            break;

        case 'K': // up
        case 'k':
            rot_x = (rot_x - crement) % 360;
            break;
            // end of view rotation

            // cube movements

            // select cube face
            // x-axis faces
        case 'Q':
        case 'q':
            reset_selected_face();
            x_0 = 0;
            x_k = 0;
            break;

        case 'W':
        case 'w':
            reset_selected_face();
            x_0 = 1;
            x_k = 1;
            break;

        case 'E':
        case 'e':
            reset_selected_face();
            x_0 = 2;
            x_k = 2;
            break;

            // y-axis faces
        case 'A':
        case 'a':
            reset_selected_face();
            y_0 = 0;
            y_k = 0;
            break;

        case 'S':
        case 's':
            reset_selected_face();
            y_0 = 1;
            y_k = 1;
            break;

        case 'D':
        case 'd':
            reset_selected_face();
            y_0 = 2;
            y_k = 2;
            break;

            // z-axis faces
        case 'C':
        case 'c':
            reset_selected_face();
            z_0 = 0;
            z_k = 0;
            break;

        case 'X':
        case 'x':
            reset_selected_face();
            z_0 = 1;
            z_k = 1;
            break;

        case 'Z':
        case 'z':
            reset_selected_face();
            z_0 = 2;
            z_k = 2;
            break;

            // move selected face
        case 'U': // counter-clockwise
        case 'u':
            apply_rotation(-90);
            break;

        case 'O': // clockwise
        case 'o':
            apply_rotation(90);
            break;

            // end of cube movements

        default:
            break;

    }

    glutPostRedisplay();

}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Project_Rubik-cube-OpenGL");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape_func);
    glutKeyboardFunc(keyboard_func);
    glutMainLoop();
} // main