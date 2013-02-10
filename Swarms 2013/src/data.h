//data.h

#ifndef __data_h__
#define __data_h__
#include <math.h>
#ifdef __MING32__
#include <gl/glut.h>
#else
    #ifdef __APPLE__
    #include <GLUT/glut.h>
    #else
    #include <GL/glut.h>
    #endif
#endif

#define MAX_AXIS_CHANGE 24.0


class myVector
{
public:

   double x,y,z;

   double mag(void) const;
   void norm(void);
   void random(void);
   void random(const double &n);
   void set(const double &a, const double &b, const double &c);
   void rotatev(const double &angle, const myVector &n);
   friend double angleBetween(const myVector &a, const myVector &b);
   friend double componentIn(const myVector &a, const myVector &b);
   friend double dot(const myVector &a, const myVector &b);
   double componentIn(const myVector &n) const;
   double dot(const myVector &n) const;
	void printDump(void) const;

   myVector operator*(const myVector &n) const;
   myVector operator*(const double &n) const;
   myVector operator*=(const double &n);
   myVector operator+(const myVector &n) const;
   myVector operator+=(const myVector &n);
   myVector operator-(const myVector &n) const;
   myVector operator-=(const myVector &n);

   myVector(void);
   myVector(const double &a, const double &b, const double &c);
   myVector(const double*);   //Reference to array: "double* &n" ??
   ~myVector(void);

};
typedef myVector vec;




class myMatrix
{
private:
   GLfloat *ptr;  //elements 0 - 15
   double rAxis;  //degrees CCW from Up about the Head vector
   vec get(const int pos) const;
protected:

   void setHead(const vec &h); //elements 0 - 3
   void setLeft(const vec &l); //elements 4 - 7
   void setUp(const vec &u);   //elemenets 8 - 11
	void set(const vec &v, const int pos);
	GLfloat* newIdentity(void);

public:
	void printDump(void);

   myMatrix(void);
   myMatrix(GLfloat* arrayPtr);
//   myMatrix(vec h);  //use mtx::random instead
   ~myMatrix(void);

   vec getHead(void) const; //elements 0 - 3
   vec getLeft(void) const; //elements 4 - 7
   vec getUp(void) const;   //elements 8 - 11

   double getr(void) const;   //returns rAxis as angle from 'up'
   vec getrAxis(void) const;   //returns rAxis as vector

   void reset(const vec& h, const vec& l, const vec& u);
   void applyRotate(void);
   //Angle is always in degrees
   void rotatev(const double &angle, const vec &axis);
   void rotateToward(const double &maxAngle, const vec &target);
   void random(void);
};
typedef myMatrix mtx;

#endif
