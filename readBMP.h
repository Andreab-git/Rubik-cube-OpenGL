#ifndef READBMP_H_INCLUDED
#define READBMP_H_INCLUDED

#include <vector>
#include <cstdio>


#define GL_SILENCE_DEPRECATION

/* ########################################## */
/* Scommentare per impostare librerie macos */

// #define APPLE

/* ########################################## */

#ifdef APPLE
#include <GLUT/glut.h>
#else
    #include <GL/glut.h>
    #include <GL/gl.h>
#endif

struct BitMapFile
{
   int sizeX;
   int sizeY;
   unsigned char *data;
};

struct BitMapFile *readBMP(const char* filename);


#endif // READBMP_H_INCLUDED
