// Rubik Cube in OpenGL

// TODO: VEDERE SE SI POSSONO METTERE I COMANDI IN UN ALTRO HEADER

#include "readBMP.h"
#include "data_path.h"
#include "coordinates.h"

using namespace std;

struct cube_data {
    GLfloat angle, x, y, z;
};

static struct BitMapFile *images[NIMAGES];
static GLenum textureID[NIMAGES];

unsigned char status_sel = 0, first_move = 0;  // "boolean" for enable / disable selected faces
GLfloat near_val = 1.4, // Frustum near val
eyeZ = 250;
GLint rot_x, rot_y, mov_steps, x_0 = 0, x_k = 0, y_0 = 0, y_k = 2, z_0 = 0, z_k = 2;
vector<cube_data> cube_rotations[3][3][3];

// init lighting
GLfloat lightAmb[4] = {0.8, 0.8, 0.8, 1.0};
GLfloat lightDiff[4] = {0.8, 0.8, 0.8, 1.0};
GLfloat lightSpec[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat lightPos[4] = {0.0, 150.0, 50.0, 1.0};

void update_rotation(GLfloat rot_angle)
{
    int step_up, step_right;
    cube_data rotation{};

    // apply rotation to cube
    for (step_up = 0; step_up < 3; step_up++)
        for (step_right = 0; step_right < 3; step_right++) {

            if (x_0 == x_k) {
                rotation = {rot_angle, 1.0, 0.0, 0.0};
                cube_rotations[x_k][step_up][step_right].push_back(rotation);
            } else if (y_0 == y_k) {
                rotation = {rot_angle, 0.0, 1.0, 0.0};
                cube_rotations[step_up][y_k][step_right].push_back(rotation);
            } else if (z_0 == z_k) {
                rotation = {-1 * rot_angle, 0.0, 0.0, 1.0};
                cube_rotations[step_up][step_right][z_k].push_back(rotation);
            }
        }

}

void reset_selected_face(void)
{
    if (!first_move)
        status_sel = 1;
    else status_sel = 0;
    x_0 = 0;
    x_k = 2;
    y_0 = 0;
    y_k = 2;
    z_0 = 0;
    z_k = 2;
}

void gen_skyboxes(void)
{
    unsigned int indFace;

    for (indFace = 6; indFace < (NFACES*2); indFace++) {        // index starts from 6 (modified textures)
        glBindTexture(GL_TEXTURE_2D, textureID[12]);
        glDrawArrays(GL_TRIANGLE_STRIP,  indFace*NVERTICES, 4);
        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void draw_cube(int x, int y, int z)
{
    unsigned int indFace;

    vector<cube_data> cube_story = cube_rotations[x][y][z];

    glPushMatrix();

    // translate to final position
    glTranslatef((x - 1) * CUBESIZE, (y - 1) * CUBESIZE, (z - 1) * CUBESIZE);

    // generate skyboxes from the center of the cube
    if(x==1 && y==1 && z==1)
        gen_skyboxes();

    // rotate cube to correct position
    for (int i = (int)(cube_story.size() - 1); i >= 0; i--)
        glRotatef(cube_story[i].angle, cube_story[i].x, cube_story[i].y, cube_story[i].z);

    for (indFace=0; indFace<NFACES; indFace++) {

        if (status_sel) {

            if (x == x_0 && x_0 == x_k) {
                if ((indFace == 0 || indFace == 2 || indFace == 4 || indFace == 5))
                    glBindTexture(GL_TEXTURE_2D, textureID[indFace + 6]);
                else glBindTexture(GL_TEXTURE_2D, textureID[indFace]);
            } else if (y == y_0 && y_0 == y_k) {
                if (indFace == 0 || indFace == 1 || indFace == 2 || indFace == 3)
                    glBindTexture(GL_TEXTURE_2D, textureID[indFace + 6]);
                else glBindTexture(GL_TEXTURE_2D, textureID[indFace]);
            } else if (z == z_0 && z_0 == z_k) {
                if (indFace == 1 || indFace == 3 || indFace == 4 || indFace == 5)
                    glBindTexture(GL_TEXTURE_2D, textureID[indFace + 6]);
                else glBindTexture(GL_TEXTURE_2D, textureID[indFace]);
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
    gluLookAt(0, 80, eyeZ, 0, 0, 0, 0, 1, 0);

    // apply visualization transformations
    glRotatef(rot_x, 1.0, 0.0, 0.0);
    glRotatef(rot_y, 0.0, 1.0, 0.0);

    for (int x = 0; x < 3; x++) // step through x axis
        for (int y = 0; y < 3; y++) // step through y axis
            for (int z = 0; z < 3; z++)  // step through z axis
                draw_cube(x, y, z);

    // flush opengl commands (swaps the buffers of the current window if double buffered)
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
            if (eyeZ >= 150) eyeZ -= 10;
            break;

        case '-':
            if (eyeZ <= 255) eyeZ += 10;
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
            first_move = 1;
            status_sel = 0;
            break;

        case 'o':
            update_rotation(90);
            first_move = 1;
            status_sel = 0;
            break;

        case 'r':
            for(int i = 0; i < 3; i++)
                for(int j = 0; j < 3; j++)
                    for(int k = 0; k < 3; k++)
                        cube_rotations[i][j][k].clear();
        default:
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
    glutAddMenuEntry("resetta cubo --- > r", 'r');

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
    mov_steps = 5; // rotation step

    makeMenu();

    // material brightness capacity
    GLfloat specularity[4] = {1.0, 1.0, 1.0, 1.0};
    GLint material_shininess = 80;

    // black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // material specularity
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularity);
    // brightness concentration
    glMateriali(GL_FRONT, GL_SHININESS, material_shininess);

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

    GLenum glErr;

    // specify projection coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // specify projection perspective
    glFrustum(-1,1,-1,1,near_val,750);

    // ... it does not hurt to check that everything went well
    if ((glErr=glGetError()) != 0) {
        printf("Errore = %d \n", glErr);
        exit(-1);
    }

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
            if (eyeZ >= 150) eyeZ -= 5;
            break;

        case '-':
            if (eyeZ <= 255) eyeZ += 5;
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

            // rotational movement
        case 'L':
        case 'l':
            rot_y = (rot_y - mov_steps) % 360;
            break;

        case 'J':
        case 'j':
            rot_y = (rot_y + mov_steps) % 360;
            break;

        case 'I':
        case 'i':
            rot_x = (rot_x + mov_steps) % 360;
            break;

        case 'K':
        case 'k':
            rot_x = (rot_x - mov_steps) % 360;
            break;

            // move selected face
        case 'U': // counter-clockwise
        case 'u':
            update_rotation(-90);
            first_move = 1;
            status_sel = 0;
            break;

        case 'O': // clockwise
        case 'o':
            update_rotation(90);
            first_move = 1;
            status_sel = 0;
            break;
            // end of cube movements

        case 'R':
        case 'r':
            for (int i = 0; i < 3; i++)
                for(int j = 0; j < 3; j++)
                    for(int k = 0; k < 3; k++)
                        cube_rotations[i][j][k].clear();
            break;
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
            rot_x = (rot_x - mov_steps) % 360;
            break;
        case GLUT_KEY_DOWN:
            rot_x = (rot_x + mov_steps) % 360;
            break;
        case GLUT_KEY_LEFT:
            rot_y = (rot_y + mov_steps) % 360;
            break;
        case GLUT_KEY_RIGHT:
            rot_y = (rot_y - mov_steps) % 360;
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
    glutMainLoop();

    return 0; /* ANSI C requires main to return int. */
}