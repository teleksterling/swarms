//world.cc
#include "world.h"
#ifdef __MING32__
#include <gl/glut.h>
#else
    #ifdef __APPLE__
    #include <GLUT/glut.h>
    #else
    #include <GL/glut.h>
    #endif
#endif

Cworld::Cworld(void)
:o(0.0, 0.0, 0.0)
{
   status = PAUSED;
   speed = 1;
}   

void Cworld::drawMe(void)
{
	 glCallList(listID);
}

vec Cworld::origin(void)
{
	return o;
}

statusEnum Cworld::getStatus(void)
{
   return status;
}
   
void Cworld::flipStatus(void)
{
   status = (status == RUNNING) ? PAUSED : RUNNING;
}   



