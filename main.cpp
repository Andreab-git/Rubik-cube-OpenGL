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
#include "main.h"

using namespace std;

struct cube_rotate {
    GLfloat angle, x, y, z;
};

static struct BitMapFile *images[NIMAGES];
static GLenum textureID[NIMAGES];

GLfloat angle, fAspect;
GLint rot_x, rot_y, crement, x_0, x_k, y_0, y_k, z_0, z_k, gap;
vector<cube_rotate> cube_rotations[3][3][3];

// init lighting
GLfloat lightAmb[4] = {0.2, 0.2, 0.2, 1.0};
GLfloat lightDiff[4] = {0.7, 0.7, 0.7, 1.0};        // color
GLfloat lightSpec[4] = {1.0, 1.0, 1.0, 1.0};    // brightness
GLfloat lightPos[4] = {0.0, 50.0, 50.0, 1.0};

/* L'ho commentata perche' sembra non servire... scritta cosi' e' solo un prototipo */
// void view_parameters(void);

void update_rotation(GLfloat angle) {

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

void camera_opt() {
    gluLookAt(0, 80, 200, 0, 0, 0, 0, 1, 0);
}

// draw a cube
void draw_cube(int x, int y, int z) {

    unsigned int indFace;

    vector<cube_rotate> lrot = cube_rotations[x][y][z];

    glPushMatrix();

    // translate to final position
    glTranslatef((x - 1) * cube_size + x * gap, (y - 1) * cube_size + y * gap, (z - 1) * cube_size + z * gap);

    // rotate cube to correct position
    for (int i = lrot.size() - 1; i >= 0; --i)
        glRotatef(lrot[i].angle, lrot[i].x, lrot[i].y, lrot[i].z);

    for (indFace=0; indFace<NFACES; indFace++) {
        // Activate texture object.
        glBindTexture(GL_TEXTURE_2D, textureID[indFace]);
        glDrawArrays(GL_TRIANGLE_STRIP, indFace * NVERTICES, 4);
        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }




    glPopMatrix();

} // draw cube function

// draw function
void display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // reset transformations
    glLoadIdentity();

    // set camera position
    camera_opt();

    // apply visualization transformations
    glRotatef(rot_x, 1.0, 0.0, 0.0); // rotate in y axis
    glRotatef(rot_y, 0.0, 1.0, 0.0); // rotate in x axis

    for (int i = 0; i < 3; ++i) // step through x axis
        for (int j = 0; j < 3; ++j) // step through y axis
            for (int k = 0; k < 3; ++k) { // step through z axis

                // draw a single cube
                draw_cube(i, j, k);
                /* Messo per capire quali sono le coordinate dei vari cubi */
                //  printf("i %d , j %d, k %d\n", i, j, k);

            }

    // flush opengl commands
    glutSwapBuffers();

}

void loadExternalTextures() {

    // local variables
    int currInd;

    // read bitmap image
    for(currInd=0; currInd<NIMAGES; currInd++) {
        // Load external textures
        images[currInd] = readBMP(fileName[currInd]);
        if(images[currInd] == NULL) {
            printf("Could not open file %s\n", fileName[currInd]);
        } else {
            printf("Loaded image %s - %d x %d pixels \n", fileName[currInd],
                   images[currInd]->sizeX, images[currInd]->sizeY);
        }
    }

    // initialize texture ID for Rubik's Cube
    glGenTextures(12, textureID);

    // read the first 6 bitmap images for the Rubik's Cube
    for (currInd=0; currInd < (NIMAGES - 6); currInd++) {
        // define parameters for texture[0] (orange face)
        // Bind image to texture object texture[0].
        glBindTexture(GL_TEXTURE_2D, textureID[currInd]);
        // Turn on OpenGL texture unit
        glEnable(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[currInd]->sizeX, images[currInd]->sizeY, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, images[currInd]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // Specify how texture values combine with current surface color values.
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}

// init rendering parameters
void init(void) {

    // init parameters

    rot_x = 0.0; // view rotation x
    rot_y = 0.0; // view rotation y
    crement = 5; // rotation (in/de)crement
    angle = 45;
    gap = 1;        // TODO : DA RIMUOVERE PRIMA O POI


    // material brightness capacity
    GLfloat specularity[4] = {1.0, 1.0, 1.0, 1.0};
    GLint material_specularity = 60;

    // black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // material reflectability
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularity);
    // brightness concentration
    glMateriali(GL_FRONT, GL_SHININESS, material_specularity);

    // activate ambient light
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmb);

    // Enable local viewpoint.
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    // define light parameters
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

//    // enable changing material color    COOOOMMMMEEENNTTTAATTTOOO PER RIMUOVERE IL COLORE
//    glEnable(GL_COLOR_MATERIAL);

    // Gouraud colorization model
    glShadeModel(GL_SMOOTH);

    // enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // enable depth buffering
    glEnable(GL_DEPTH_TEST);

    loadExternalTextures();

    // initialize model view transforms
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

// Set vertex arrays for coordinates, normals and texture coordinates
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertexCoords);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, tNormals);
    // Activate client state for texturing
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, textCoords);

} // init

// specify what's shown in the window
void view_parameters(void) {
    GLenum glErr;

    // specify projection coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // specify projection perspective
    gluPerspective(angle, fAspect, 0.4, 500);

    // ... it does not hurt to check that everything went well
    if ((glErr=glGetError()) != 0) {
        printf("Errore = %d \n", glErr);
        exit(-1);
    }

    // init model coordinate system
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // specify observer and target positions
    camera_opt();
} // load visualization parameters

// window reshape callback
void reshape(GLsizei w, GLsizei h) {
    // prevents division by zero
    if (h == 0) h = 1;

    // viewport size
    glViewport(0, 0, w, h);

    // aspect ratio
    fAspect = (GLfloat) w / (GLfloat) h;

    view_parameters();
} // reshape function

// keyboard function callback
void keyInput(unsigned char key, int x, int y) {

    switch (key) {

        case 27:
            // free allocated memory
            for (int i=0; i<NIMAGES; i++)
                free(images[i]->data);
            // ESC
            exit(0);
        case '+':
            if (angle >= 10) angle -= 5;
            view_parameters();
            break;

        case '-':
            if (angle <= 130) angle += 5;
            view_parameters();
            break;
            // vielw rotation
            // INcrement or DEcrement


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
            update_rotation(-90);
            break;

        case 'O': // clockwise
        case 'o':
            update_rotation(90);
            break;
            // end of cube movements

        default:
            break;
    }

    glutPostRedisplay();
}

void SpecialInput(int key, int x, int y)
{
    //view rotation
    //increment or decrement
    switch(key)
    {
        case GLUT_KEY_UP:
            rot_x = (rot_x - crement) % 360;
            break;
        case GLUT_KEY_DOWN:
            rot_x = (rot_x + crement) % 360;
            break;
        case GLUT_KEY_LEFT:
            rot_y = (rot_y + crement) % 360;
            break;
        case GLUT_KEY_RIGHT:
            rot_y = (rot_y - crement) % 360;
            break;
    }

    glutPostRedisplay();
}

int main(int argc, char **argv) {

    // pass potential input arguments to glutInit
    glutInit(&argc, argv);

    // set display mode
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);


    glutInitWindowSize(400, 400);
    glutCreateWindow("Project_Rubik-cube-OpenGL");

    // keyboard handling function
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(SpecialInput);

    // Call initialization routines
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0; /* ANSI C requires main to return int. */
} // main