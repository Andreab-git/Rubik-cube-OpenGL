// Rubik Cube in OpenGL

#include "readBMP.h"
#include "data_path.h"
#include "coordinates.h"

using namespace std;

struct cube_rotate {
    GLfloat angle, x, y, z;
};

static struct BitMapFile *images[NIMAGES];
static GLenum textureID[NIMAGES];

unsigned char status_sel=0;  // "boolean" for enable / disable selected faces
GLfloat angle, fAspect;
GLint rot_x, rot_y, crement, x_0 = 0, x_k = 0, y_0 = 0, y_k = 2, z_0 = 0, z_k = 2;
vector<cube_rotate> cube_rotations[3][3][3];

// init lighting
GLfloat lightAmb[4] = {0.8, 0.8, 0.8, 1.0};
GLfloat lightDiff[4] = {0.8, 0.8, 0.8, 1.0};
GLfloat lightSpec[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat lightPos[4] = {0.0, 150.0, 50.0, 1.0};

void update_rotation(GLfloat rot_angle)
{
    vector<cube_rotate> face[3][3];
    int index;
    cube_rotate rotation;

    // copy face to be rotated
    // apply rotation to face
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {

            index = 2 - j % 3;

            if (x_0 == x_k) {
                rotation = {rot_angle, 1.0, 0.0, 0.0};
                face[index][i] = cube_rotations[x_k][i][j];
            }

            if (y_0 == y_k) {
                rotation = {rot_angle, 0.0, 1.0, 0.0};
                face[index][i] = cube_rotations[j][y_k][i];
            }

            if (z_0 == z_k) {
                rotation = {-1 * rot_angle, 0.0, 0.0, 1.0};
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
void reset_selected_face()
{
    status_sel=1;
    x_0 = 0;
    x_k = 2;
    y_0 = 0;
    y_k = 2;
    z_0 = 0;
    z_k = 2;
}

void camera_opt()
{
    gluLookAt(0, 80, 200, 0, 0, 0, 0, 1, 0);
}

//draw skyboxes
void gen_skyboxes(void)
{
    unsigned int indFace;

    for (indFace=6; indFace<(NFACES*2); indFace++) {
        glBindTexture(GL_TEXTURE_2D, textureID[12]);
        glDrawArrays(GL_TRIANGLE_STRIP,  indFace*NVERTICES, 4);
        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

// draw a cube
void draw_cube(int x, int y, int z)
{
    unsigned int indFace;

    vector<cube_rotate> lrot = cube_rotations[x][y][z];

    glPushMatrix();

    // translate to final position
    glTranslatef((x - 1) * CUBESIZE, (y - 1) * CUBESIZE, (z - 1) * CUBESIZE);

    // generate skyboxes from the center of the cube
    if(x==1 && y==1 && z==1)
        gen_skyboxes();

    // rotate cube to correct position
    for (int i = lrot.size() - 1; i >= 0; --i)
        glRotatef(lrot[i].angle, lrot[i].x, lrot[i].y, lrot[i].z);

    for (indFace=0; indFace<NFACES; indFace++) {

        if (status_sel) {
            if ((x == x_0 && x_0 == x_k) && (indFace == 0 || indFace == 2 || indFace == 4 || indFace == 5)) {
                glBindTexture(GL_TEXTURE_2D, textureID[indFace + 6]);
            } else if ((x == x_0 && x_0 == x_k) && (indFace == 0 || indFace == 2 || indFace == 4 || indFace == 5)) {
                glBindTexture(GL_TEXTURE_2D, textureID[indFace + 6]);
            } else if ((x == x_0 && x_0 == x_k) && (indFace == 0 || indFace == 2 || indFace == 4 || indFace == 5)) {
                glBindTexture(GL_TEXTURE_2D, textureID[indFace + 6]);
            } else if ((y == y_0 && y_0 == y_k) && (indFace == 0 || indFace == 1 || indFace == 2 || indFace == 3)) {
                glBindTexture(GL_TEXTURE_2D, textureID[indFace + 6]);
            } else if ((y == y_0 && y_0 == y_k) && (indFace == 0 || indFace == 1 || indFace == 2 || indFace == 3)) {
                glBindTexture(GL_TEXTURE_2D, textureID[indFace + 6]);
            } else if ((y == y_0 && y_0 == y_k) && (indFace == 0 || indFace == 1 || indFace == 2 || indFace == 3)) {
                glBindTexture(GL_TEXTURE_2D, textureID[indFace + 6]);
            } else if ((z == z_0 && z_0 == z_k) && (indFace == 1 || indFace == 3 || indFace == 4 || indFace == 5)) {
                glBindTexture(GL_TEXTURE_2D, textureID[indFace + 6]);
            } else if ((z == z_0 && z_0 == z_k) && (indFace == 1 || indFace == 3 || indFace == 4 || indFace == 5)) {
                glBindTexture(GL_TEXTURE_2D, textureID[indFace + 6]);
            } else if ((z == z_0 && z_0 == z_k) && (indFace == 1 || indFace == 3 || indFace == 4 || indFace == 5)) {
                glBindTexture(GL_TEXTURE_2D, textureID[indFace + 6]);
            } else { glBindTexture(GL_TEXTURE_2D, textureID[indFace]); }

            glDrawArrays(GL_TRIANGLE_STRIP, indFace * NVERTICES, 4);
            // unbind texture
            glBindTexture(GL_TEXTURE_2D, 0);

        } else { // draw cube with standard textures

            glBindTexture(GL_TEXTURE_2D, textureID[indFace]);
            glDrawArrays(GL_TRIANGLE_STRIP, indFace * NVERTICES, 4);
            // unbind texture
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    glPopMatrix();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // reset transformations
    glLoadIdentity();

    // set camera position
    camera_opt();

    // apply visualization transformations
    glRotatef(rot_x, 1.0, 0.0, 0.0);
    glRotatef(rot_y, 0.0, 1.0, 0.0);

    for (int i = 0; i < 3; ++i) // step through x axis
        for (int j = 0; j < 3; ++j) // step through y axis
            for (int k = 0; k < 3; ++k)  // step through z axis
                draw_cube(i, j, k);

    // flush opengl commands
    glutSwapBuffers();
}

void loadExternalTextures(void)
{
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
    glGenTextures(NIMAGES, textureID);

    // read the first 6 bitmap images for the Rubik's Cube
    for (currInd=0; currInd < NIMAGES ; currInd++) {
        // define parameters for texture[0] (orange face)
        // Bind image to texture object texture[0].
        glBindTexture(GL_TEXTURE_2D, textureID[currInd]);
        // Turn on OpenGL texture unit
        glEnable(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[currInd]->sizeX, images[currInd]->sizeY, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, images[currInd]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Specify how texture values combine with current surface color values.
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}

// specify what's shown in the window
void view_parameters(void)
{
    GLenum glErr;

    // specify projection coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // specify projection perspective
    gluPerspective(angle, fAspect, 0.4, 700);

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
}


void top_menu(int id)
{
    // Exit if the user selected the "quit" option
    if (id == 1) {
        // free allocated memory
        for (int i = 0; i < NIMAGES; i++)
            free(images[i]->data);
        exit(0);
    }
}

void game_manual(int id)
{
    switch (id) {
        case '+':
            if (angle >= 10) angle -= 5;
            view_parameters();
            break;

        case '-':
            if (angle <= 80) angle += 5;
            view_parameters();
            break;

            // x-axis faces
        case 'q':
            reset_selected_face();
            x_0 = 0;
            x_k = 0;
            break;

        case 'w':
            reset_selected_face();
            x_0 = 1;
            x_k = 1;
            break;

        case 'e':
            reset_selected_face();
            x_0 = 2;
            x_k = 2;
            break;

            // y-axis faces
        case 'a':
            reset_selected_face();
            y_0 = 0;
            y_k = 0;
            break;

        case 's':
            reset_selected_face();
            y_0 = 1;
            y_k = 1;
            break;

        case 'd':
            reset_selected_face();
            y_0 = 2;
            y_k = 2;
            break;

            // z-axis faces
        case 'c':
            reset_selected_face();
            z_0 = 0;
            z_k = 0;
            break;

        case 'x':
            reset_selected_face();
            z_0 = 1;
            z_k = 1;
            break;

        case 'z':
            reset_selected_face();
            z_0 = 2;
            z_k = 2;
            break;

            // move selected face
        case 'u':
            update_rotation(-90);
            status_sel=0;
            break;

        case 'o':
            update_rotation(90);
            status_sel=0;
            break;
    }

    glutPostRedisplay();
}

void makeMenu(void)
{
    // The sub-menu is created first (because it should be visible when the top menu is created)
    int sub_menu;

    sub_menu = glutCreateMenu(game_manual);
    glutAddMenuEntry("avvicina il cubo --- > +", '+');
    glutAddMenuEntry("allontana il cubo --- > -", '-');
    glutAddMenuEntry("sposta prima colonna in x --- > q", 'q');
    glutAddMenuEntry("sposta seconda colonna in x --- > w", 'w');
    glutAddMenuEntry("sposta terza colonna in x --- > e", 'e');
    glutAddMenuEntry("sposta prima riga in y --- > a", 'a');
    glutAddMenuEntry("sposta seconda riga in y --- > s", 's');
    glutAddMenuEntry("sposta terza riga in y --- > d", 'd');
    glutAddMenuEntry("sposta prima colonna in z --- > ", 'c');
    glutAddMenuEntry("sposta seconda colonna in z --- > ", 'x');
    glutAddMenuEntry("sposta terza colonna in z --- > ", 'z');
    glutAddMenuEntry("sposta in senso orario --- > u", 'u');
    glutAddMenuEntry("sposta in senso antiorario --- > o", 'o');

    // The top menu is created: its callback function is registered and menu entries, including a submenu, added.
    glutCreateMenu(top_menu);
    glutAddSubMenu("Comandi", sub_menu);
    glutAddMenuEntry("Quit",1);

    // The menu is attached to a mouse button.
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init(void)
{
    // init parameters
    rot_x = 0; // view rotation x
    rot_y = 0; // view rotation y
    crement = 5; // rotation (in/de)crement
    angle = 60;

    makeMenu();

    // material brightness capacity
    GLfloat specularity[4] = {1.0, 1.0, 1.0, 1.0};
    GLint material_specularity = 80;

    // black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // material specularity
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
}

void reshape(GLsizei w, GLsizei h)
{
    // prevents division by zero
    if (h == 0) h = 1;

    // viewport size
    glViewport(0, 0, w, h);

    // aspect ratio
    fAspect = (GLfloat) w / (GLfloat) h;

    view_parameters();
} // reshape function

// keyboard function callback
void keyInput(unsigned char key, int x, int y)
{
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
            if (angle <= 80) angle += 5;
            view_parameters();
            break;

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
            status_sel=0;
            break;

        case 'O': // clockwise
        case 'o':
            update_rotation(90);
            status_sel=0;
            break;
            // end of cube movements

        default:
            break;
    }

    glutPostRedisplay();
}

void SpecialInput(int key, int x, int y)
{
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

int main(int argc, char **argv)
{
    // pass potential input arguments to glutInit
    glutInit(&argc, argv);

    // set display mode
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(600, 600);
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