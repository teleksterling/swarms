//camera.cc

#include "camera.h"


vec cam::getEye(void) const
{
   vec p(pos.x, pos.y, pos.z);
   return p;
}

vec cam::getLookat(void) const
{
   return getHead();
}

//Inherited from mtx
//vec cam::getUp(void) const
//{
//   return myMatrix::getUp();
//}


void cam::setEye(const double &x, const double &y, const double &z)
{
   pos.set(x, y, z);
}

void cam::setLookat(const double &x, const double &y, const double &z)
{
   vec h(x, y, z);
   mtx::setHead(h);
   //left vector = head x up
   setLeft(getUp() * h);
}

void cam::setUp(const double &x, const double &y, const double &z)
{
   vec u(x, y, z);
   mtx::setUp(u);
   //left vector = up x head
   setLeft(u * getHead());
}



void cam::aspect(float a)
{
   if(a > 0)
   {
      aspectRatio = a;
   }   
}
   
float cam::aspect(void)
{
   return aspectRatio;   
}   


void cam::zoomIn(void)
{
	fov = fmax(fov * 0.9, 1.0);
}

void cam::zoomOut(void)
{
	fov = fmin(fov * 1.11111, 170.0);
}

void cam::zoom(float ang)
{
	fov = fmax(fmin(ang, 170.0), 1.0);
}

float cam::zoom(void)
{
	return fov;
}

