

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <ctime>
#ifdef __MING32__
#include <gl/glut.h>
#else
   #ifdef __APPLE__
   #include <GLUT/glut.h>
   #else
   #include <GL/glut.h>
   #endif
#endif

void setCamera(void);
