//object.cc
#include <stdio.h>
#include <iostream>
#include "object.h"
#include "defines.h"
#ifdef __MING32__
#include <gl/glut.h>
#else
    #ifdef __APPLE__
    #include <GLUT/glut.h>
    #else
    #include <GL/glut.h>
    #endif
#endif
using std::cout;
using std::endl;


///////////////////
//     object
///////////////////


object::object(void)
:pos(), HLU()
{
    vel = INIT_VEL;
}

object::object(vec p)
:pos(p)
{
    vel = INIT_VEL;
}

/*
object::object(vec p, vec h)
:pos(p), HLU(h)
{
    vel = INIT_VEL;


}
*/

double object::getX(void) const
{
   return pos.x;
}   

double object::getY(void) const
{
   return pos.y;
}   

double object::getZ(void) const
{
   return pos.z;
}   

vec object::getPos(void) const
{
   return pos;
}

vec object::getHead(void) const
{
   return HLU.getHead();  
}
   
vec object::getUp(void) const
{
   return HLU.getUp();  
}   

double object::getr(void) const
{
   return HLU.getr();
}

vec object::getrAxis(void) const
{
   return HLU.getrAxis();
}

void object::random(void)
{
	pos.random(80.0);
   //pos = pos *10.0;
	HLU.random();
    
}

vec object::getAvoidUrge(const object& activeBoid)
{
   return bound->doIntersect(pos, activeBoid.pos, activeBoid.getHead());
}

void object::set(const vec& p, const vec& h, const vec& l, const vec& u)
{
   vec v;

   pos.set(p.x, p.y, p.z);
   HLU.reset(h,l,u);

   //cout << endl;
   //pos.printDump();
   //HLU.printDump();

}

void object::drawMe(void)
{
	vec h = HLU.getHead(),
   	l = HLU.getLeft(),
      u = HLU.getHead() * HLU.getLeft(),
      a = HLU.getrAxis();

   glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	 glTranslatef(pos.x, pos.y, pos.z);
//    glBegin(GL_LINES);
//     //head
//     glColor3f(1.0, 1.0, 0.0);
//     glVertex3f(0.0, 0.0, 0.0);
//     glVertex3f(h.x*2, h.y*2, h.z*2);
//     //left
//     glColor3f(1.0, 0.0, 0.0);
//     glVertex3f(0.0, 0.0, 0.0);
//     glVertex3f(l.x, l.y, l.z);
//     //up
//     glColor3f(1.0, 0.0, 0.0);
//     glVertex3f(0.0, 0.0, 0.0);
//     glVertex3f(u.x, u.y, u.z);
//     //position
//     glColor3f(1.0, 1.0, 1.0);
//     glVertex3f(0.0, 0.0, 0.0);
//     glVertex3f(pos.x*-1, pos.y*-1, pos.z*-1);
//     //turn axis
//     glColor3f(0.0, 0.0, 1.0);
//     glVertex3f(0.0, 0.0, 0.0);
//     glVertex3f(a.x, a.y, a.z);
//    glEnd();
	 HLU.applyRotate();
	 //bound->drawMe();
	 glCallList(listID);
   glPopMatrix();
	glPopAttrib();
    //cout << pos.z << "   ";
}

void object::updateMe(void)
{

}

////////////////////
//     sphere
////////////////////

void sphere::set(float rad, float x, float y, float z)
{
   radius = (rad < 1.0) ? 1.0 : rad;
   pos.set(x,y,z);
   bound = new boundingSphere(radius + 1.0);
   cout << "r " << radius;
   cout << ", x " << x;
   cout << ", y " << y;
   cout << ", z " << z << endl;
   listID = WIRESPHERE_ID;
}   

void sphere::drawMe(void)
{
   glMatrixMode(GL_MODELVIEW);
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glPushMatrix();
    glColor3f(0.5, 0.0, 0.0);
    glTranslatef(pos.x, pos.y, pos.z);
    glScalef(radius, radius, radius);
    glCallList(listID);
   glPopMatrix();
	glPopAttrib();
   
}   

////////////////////
//    objectBox
////////////////////

objectBox::objectBox(int type, unsigned int newCount)
{
   if(type == WIRESPHERE_ID)
   { // this is the only currently implemented static object
 	   list = new sphere[newCount];
 	   count = newCount;
   }   
}   

objectBox::~objectBox(void)
{
/*
   //This fragment is used if 'list' is an array of pointers,
   // to allow different types of objects to be stored in the
   // same objectBox.
   int i;
   for(i = 0; i < count; i++)
   {  //delete all pointered objects
      delete list[i];
   }
   //delete the pointers!
*/
   delete[] list;
}   
    
object& objectBox::operator[](int i)
{
   return list[i < count ? (i > 0 ? i : 0) : count];
}



////////////////////
//Bounding Objects
////////////////////
//Bounding Sphere
////////////////////

boundingSphere::boundingSphere(void)
:radius(1.0)
{
}

boundingSphere::boundingSphere(const double& rad)
:radius(rad)
{
}

void boundingSphere::setRadius(float rad)
{
   radius = (rad < 1.0) ? 1.0 : rad;
}   

vec boundingSphere::doIntersect(const vec& C, const vec& P, const vec& H)
{
   //The Ray-Sphere intersection algorithm used here
   // is based on the algebraic description on this website:
   // http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter1.htm
   // G. Scott Owen, accessed 6/10/04

   //Assumption - h is of unit length
   // C = center of the boundingSphere
   // P = Position of Boid
   // H = Unit Heading Vector of Boid

   //Return value is always a vector of length =
   //  (radius of bound) / (distance from bound centre to boid)
   // except when distance is 0.0, length is then 10.

   vec pMinusC = P - C;

   double pcDist = pMinusC.mag();
   if (pcDist == 0.0)
   {
      //if in the center of the sphere, then just head straight out
      return H * 10;
   }

   //b, c are the co-efficients in y = ax^2 + bx +c
   //a is 1, because we are assuming H is unit length
   double b = 2 * dot(H, pMinusC),
          c = pcDist * pcDist - radius * radius,
          desc = b*b - 4*c, //descriminant , a = 1
          sqrtDesc,  //pre-computed sqrt(desc), calculated once later if needed
          t,         //scaling factor of H to reach intersect with sphere
          rOverPCDist;  //pre-computed value radius/pcDist

   if (desc < 0)
   {
      //no intersect, no urge
      //return zero vector
      pMinusC.set(0.0, 0.0, 0.0);
      return pMinusC;
   }
   else
   {  //find intersect
      sqrtDesc = sqrt(desc);

      //negative square root solution
      //if (first intersection is behind boid)
      if (0.0 > (t = -0.5 * (b + sqrtDesc))) //assignment intentional
      {
         //positive square root solution
         //if (second intersection is behind boid)
         if (0.0 > (t += sqrtDesc))  //assignment intentional
         {
            //object is behind boid, no urge
            //return zero vector
            pMinusC.set(0.0, 0.0, 0.0);
            return pMinusC;
         }
         else
         {
            //boid is inside the object
            //return radial vector with magnitude radius / distance from center
            rOverPCDist = radius / pcDist;
            return pMinusC * (rOverPCDist * rOverPCDist);
         }
      }
      else
      {
         //return radial vector from point of intersection
         return (pMinusC + H * t) * (1.0 / pcDist);
      }
   }
}

void boundingSphere::drawMe(void)
{
   //glutWireSphere(radius, slices, stacks);
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glPushMatrix();
    glColor4f(0.0, 0.0, 0.5, 0.2);
    glScalef(radius, radius, radius);
    glCallList(WIRESPHERE_ID);
   glPopMatrix();
   glPopAttrib();
}



