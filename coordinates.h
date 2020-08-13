#define NFACES 6
#define NVERTICES 4

GLfloat background_image= 200.0;
GLfloat half_image=background_image/2;
GLfloat cube_size = 30.0; // cuboid size
GLfloat half_cube = cube_size/2;

/* ********************************************** RUBIK'S CUBE DATA ************************************************ */

// texture coordinates
GLfloat textCoords[(NFACES+1)*NVERTICES*2] =
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
        };

// vertex coordinates
GLfloat vertexCoords[(NFACES + 1) * NVERTICES * 3] = {
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

        /*background_image*/
        -half_image, -half_image, half_image,
        half_image, -half_image, half_image,
        -half_image, half_image, half_image,
        half_image, half_image, half_image,

};

// normals
GLfloat tNormals[(NFACES+1)*NVERTICES*3] =
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

                /*background_image*/
                0.0, 0.0, 1.0,
                0.0, 0.0, 1.0,
                0.0, 0.0, 1.0,
                0.0, 0.0, 1.0,
        };

/* ****************************************************************************************************************** */