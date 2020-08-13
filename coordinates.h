#define NFACES 6
#define NVERTICES 4

GLfloat skyboxe_image= 550.0;
GLfloat half_image=skyboxe_image/2;
GLfloat cube_size = 30.0; // cuboid size
GLfloat half_cube = cube_size/2;

/* ********************************************** RUBIK'S CUBE DATA ************************************************ */

// texture coordinates
GLfloat textCoords[NFACES*NVERTICES*2*2] =
        {
                0.0, 0.0,
                1.0, 0.0,
                0.0, 1.0,
                1.0, 1.0,

                0.0, 0.0,
                1.0, 0.0,
                0.0, 1.0,
                1.0, 1.0,

                0.0, 0.0,
                1.0, 0.0,
                0.0, 1.0,
                1.0, 1.0,

                0.0, 0.0,
                1.0, 0.0,
                0.0, 1.0,
                1.0, 1.0,

                0.0, 0.0,
                1.0, 0.0,
                0.0, 1.0,
                1.0, 1.0,

                0.0, 0.0,
                1.0, 0.0,
                0.0, 1.0,
                1.0, 1.0,

                0.0, 0.0,
                1.0, 0.0,
                0.0, 1.0,
                1.0, 1.0,

                0.0, 0.0,
                1.0, 0.0,
                0.0, 1.0,
                1.0, 1.0,

                0.0, 0.0,
                1.0, 0.0,
                0.0, 1.0,
                1.0, 1.0,

                0.0, 0.0,
                1.0, 0.0,
                0.0, 1.0,
                1.0, 1.0,

                0.0, 0.0,
                1.0, 0.0,
                0.0, 1.0,
                1.0, 1.0,

                0.0, 0.0,
                1.0, 0.0,
                0.0, 1.0,
                1.0, 1.0,
        };

// vertex coordinates
GLfloat vertexCoords[NFACES * NVERTICES * 3 *2] = {
        /* red face (front) */
        -half_cube, -half_cube, half_cube,
        half_cube, -half_cube, half_cube,
        -half_cube, half_cube, half_cube,
        half_cube, half_cube, half_cube,

        /* blue face (left) */
        -half_cube, -half_cube, half_cube,
        -half_cube, half_cube, half_cube,
        -half_cube, -half_cube, -half_cube,
        -half_cube, half_cube, -half_cube,

        /* orange face (back)*/
        -half_cube, -half_cube, -half_cube,
        half_cube, -half_cube, -half_cube,
        -half_cube, half_cube, -half_cube,
        half_cube, half_cube, -half_cube,

        /* green face (right) */
        half_cube, -half_cube, half_cube,
        half_cube, -half_cube, -half_cube,
        half_cube, half_cube, half_cube,
        half_cube, half_cube, -half_cube,

        /* white face (top) */
        -half_cube, half_cube, half_cube,
        half_cube, half_cube, half_cube,
        -half_cube, half_cube, -half_cube,
        half_cube, half_cube, -half_cube,

        /* yellow face (bottom) */
        -half_cube, -half_cube, half_cube,
        half_cube, -half_cube, half_cube,
        -half_cube, -half_cube, -half_cube,
        half_cube, -half_cube, -half_cube,

        /*skyboxe_image*/

        /*back*/
        -half_image, -half_image, -half_image,
        half_image, -half_image, -half_image,
        -half_image, half_image, -half_image,
        half_image, half_image, -half_image,

        /*front*/
        -half_image, -half_image, half_image,
        half_image, -half_image, half_image,
        -half_image, half_image, half_image,
        half_image, half_image, half_image,

        /*left*/
        /*-half_image, -half_image, half_image,
        -half_image, half_image, half_image,
        -half_image, -half_image, half_image,
        -half_image, half_image, half_image,*/
        half_image, -half_image, half_image,
        half_image, -half_image, -half_image,
        half_image, half_image, half_image,
        half_image, half_image, -half_image,

        /*right*/
        half_image, -half_image, half_image,
        half_image, -half_image, -half_image,
        half_image, half_image, half_image,
        half_image, half_image, -half_image,

        /*top*/
        -half_image, half_image, half_image,
        half_image, half_image, half_image,
        -half_image, half_image, -half_image,
        half_image, half_image, -half_image,

        /*bottom*/
        -half_image, -half_image, half_image,
        half_image, -half_image, half_image,
        -half_image, -half_image, -half_image,
        half_image, -half_image, -half_image,

};

// normals
GLfloat tNormals[NFACES*NVERTICES*3*2] =
        {
                /* red face (front) */
                0.0, 0.0, 1.0,
                0.0, 0.0, 1.0,
                0.0, 0.0, 1.0,
                0.0, 0.0, 1.0,

                /* blue face (left) */
                -1.0, 0.0, 0.0,
                -1.0, 0.0, 0.0,
                -1.0, 0.0, 0.0,
                -1.0, 0.0, 0.0,

                /* orange face (back)*/
                0.0, 0.0, -1.0,
                0.0, 0.0, -1.0,
                0.0, 0.0, -1.0,
                0.0, 0.0, -1.0,

                /* green face (right) */
                1.0, 0.0, 0.0,
                1.0, 0.0, 0.0,
                1.0, 0.0, 0.0,
                1.0, 0.0, 0.0,

                /* white face (top) */
                0.0, 1.0, 0.0,
                0.0, 1.0, 0.0,
                0.0, 1.0, 0.0,
                0.0, 1.0, 0.0,

                /* yellow face (bottom) */
                0.0, -1.0, 0.0,
                0.0, -1.0, 0.0,
                0.0, -1.0, 0.0,
                0.0, -1.0, 0.0,

                /*skyboxe_image*/

                /*back*/
                0.0, 0.0, 1.0,
                0.0, 0.0, 1.0,
                0.0, 0.0, 1.0,
                0.0, 0.0, 1.0,

                /*front*/
                0.0, 0.0, -1.0,
                0.0, 0.0, -1.0,
                0.0, 0.0, -1.0,
                0.0, 0.0, -1.0,

                /*right*/
                -1.0, 0.0, 0.0,
                -1.0, 0.0, 0.0,
                -1.0, 0.0, 0.0,
                -1.0, 0.0, 0.0,

                /*left*/
                1.0, 0.0, 0.0,
                1.0, 0.0, 0.0,
                1.0, 0.0, 0.0,
                1.0, 0.0, 0.0,

                /*top*/
                0.0, -1.0, 0.0,
                0.0, -1.0, 0.0,
                0.0, -1.0, 0.0,
                0.0, -1.0, 0.0,

                /*bottom*/
                0.0, 1.0, 0.0,
                0.0, 1.0, 0.0,
                0.0, 1.0, 0.0,
                0.0, 1.0, 0.0,
        };

/* ****************************************************************************************************************** */