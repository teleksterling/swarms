//data.cc
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "data.h"
using std::cout;
using std::endl;

//******** myVector member functions *********//

//Matrix is:	[0  4  8  12
//		 1  5  9  13
//		 2  6  10 14
//		 3  7  11 15]

double myVector::mag(void) const
{
   if( x==0.0 && y == 0.0 && z == 0.0)
   {
      return 0.0;
   }
   else
   {   
      return sqrt(x*x + y*y + z*z);
   }   
}

void myVector::norm(void)
{
   double n, m = mag();
   if(m > 0.0)
   {
      n = 1/m;

      x *= n;
      y *= n;
      z *= n;
   }   
}

void myVector::random(void)
{
   random(1.0);
}

void myVector::random(const double &n)
{
   x = (double) rand()/RAND_MAX - 0.5;
   y = (double) rand()/RAND_MAX - 0.5;
   z = (double) rand()/RAND_MAX - 0.5;

   if(mag() == 0.0)  //zero vector
   {
      x = 1.0;  //plus, no need to normalise
   }
   else
   {
      norm();  //unit vector
   }
   x *= n;
   y *= n;
   z *= n;

   //cout << "(" << x << "," << y << "," << z << ")" << endl;
}

void myVector::rotatev(const double &angle, const myVector &n)
{
   GLfloat matrix[16] = {0};

   if(mag() == 0 || n.mag() < 1e-3)
   {
      return;
   }
   else
   {
      matrix[0] = x;
      matrix[1] = y;
      matrix[2] = z;
      glPushMatrix();
      glLoadIdentity();
      glRotated(angle, n.x, n.y, n.z);
      glMultMatrixf(matrix);
      glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
      glPopMatrix();
      x = matrix[0];
      y = matrix[1];
      z = matrix[2];
   }   
}


void myVector::set(const double &a, const double &b, const double &c)
{
   x = a;
   y = b;
   z = c;
}

//friend function - angle between 2 vectors
double angleBetween(const myVector &a, const myVector &b)
{
//   cout << 180 / M_PI * acos( (a.x*b.x + a.y*b.y + a.z*b.z) / (a.mag() * b.mag()) ) << " | ";
   double val;
   if (a.mag() == 0.0 || b.mag() == 0.0)
   {
      return 0.0;
   }
   else
   {
      val = (a.x*b.x + a.y*b.y + a.z*b.z) / (a.mag() * b.mag());
//      cout << val << " , ";
      val = (val < -1) ? -1:((val > 1)? 1: val);
//      cout << val << " ." << endl;
      
      return  acos(val) * (180 / M_PI);
   }   
}
//friend function - scalar product = a.b
double dot(const myVector &a, const myVector &b)
{
   return  a.x*b.x + a.y*b.y + a.z*b.z;
}

//Scalar Product  - scalar product = a.b
double myVector::dot(const myVector &n) const
{
   return ::dot(*this, n);
}   

//friend function - scalar product = a.(b^)
double componentIn(const myVector &a, const myVector &b)
{
   if (b.mag() == 0.0)
   {
      return 0.0;
   }
   else
   {      
      return  (a.x*b.x + a.y*b.y + a.z*b.z) / b.mag();
   }   
}

//Scalar Product  - scalar product = a.(b^)
double myVector::componentIn(const myVector &n) const
{
   return ::componentIn(*this, n);
}   


//Cross product
myVector myVector::operator*(const myVector &n) const
{
   vec vp;

   vp.x = y * n.z - z * n.y;
   vp.y = z * n.x - x * n.z;
   vp.z = x * n.y - y * n.x;

   return vp;
}

//scalar multiplication
myVector myVector::operator*(const double &n) const
{
   vec vp;

   vp.x = x * n;
   vp.y = y * n;
   vp.z = z * n;

   return vp;
}

myVector myVector::operator*=(const double &n)
{
   x *= n;
   y *= n;
   z *= n;

}

myVector myVector::operator+(const myVector &n) const
{
   vec vp;

   vp.x = x + n.x;
   vp.y = y + n.y;
   vp.z = z + n.z;

   return vp;
}

myVector myVector::operator+=(const myVector &n)
{
   x += n.x;
   y += n.y;
   z += n.z;
}

myVector myVector::operator-(const myVector &n) const
{
   vec vp;

   vp.x = x - n.x;
   vp.y = y - n.y;
   vp.z = z - n.z;

   return vp;
}

myVector myVector::operator-=(const myVector &n)
{
   x -= n.x;
   y -= n.y;
   z -= n.z;
}


void myVector::printDump(void) const
{
      cout << "( " << x << ", " << y << ", " << z << ")" << endl;
}


myVector::myVector(void)
{
   x = 0.0;
   y = 0.0;
   z = 0.0;
}

myVector::myVector(const double &a, const double &b, const double &c)
{
   x = a;
   y = b;
   z = c;

}

myVector::myVector(const double* n)
{
   x = n[0];
   y = n[1];
   z = n[2];
}

myVector::~myVector(void)
{

}




















//******** myMatrix member functions *********//

vec myMatrix::getHead(void) const //elements 0-3
{return get(0);}

vec myMatrix::getLeft(void) const //elements 4-7
{return get(4);}

vec myMatrix::getUp(void) const   //elements 8-11
{return get(8);}

vec myMatrix::get(const int pos) const
{
   vec result;

   result.x = ptr[pos];
   result.y = ptr[pos + 1];
   result.z = ptr[pos + 2];

   return result;
}

double myMatrix::getr(void) const
{
   return rAxis;
}

vec myMatrix::getrAxis(void) const
{
	vec up(getUp());
   up.rotatev(rAxis, getHead());

   return up;
}

void myMatrix::printDump(void)
{
   int i, j;
   for (i = 0; i < 4; i++)
   {
      cout << "[ ";
      for (j = 0; j < 4; j++)
      {
         cout << ptr[i * 4 + j] << " ";
      }
      cout << "]" << endl;
   }
}

void myMatrix::setHead(const vec &h) //elements 0 - 3
{set(h,0);}

void myMatrix::setLeft(const vec &l) //elements 4 - 7
{set(l,4);}

void myMatrix::setUp(const vec &u)   //elements 8 - 11
{set(u,8);}

void myMatrix::set(const vec &v, const int pos)
{
   ptr[pos] = v.x;
   ptr[pos + 1] = v.y;
   ptr[pos + 2] = v.z;
   ptr[pos + 3] = 0.0;
}

void myMatrix::reset(const vec& h, const vec& l, const vec& u)
{
   
   setHead(h);
   setLeft(l);
   setUp(u);
   ptr[12] = 0.0;
   ptr[13] = 0.0;
   ptr[14] = 0.0;
   ptr[15] = 1.0;

   rAxis = 0.0;
}


GLfloat* myMatrix::newIdentity(void)
{
   int i;
   float* p = new GLfloat [16];

   for(i = 0; i < 16 ; ++i)
   {
      p[i] = (i % 5 == 0) ? 1.0 : 0.0;
   }
   return p;
}


myMatrix::myMatrix(void)
{
   ptr = newIdentity();
   rAxis = 0.0;

}

myMatrix::myMatrix(GLfloat* arrayPtr)
{
   int i;
   ptr = new GLfloat [16];

   for(i = 0; i < 16 ; ++i)
   {
      ptr[i] = arrayPtr[i];
   }
   rAxis = 0.0;

}

void myMatrix::random(void)
{
   vec n;
   //Rotate Identity by random angle about a random vector axis - simple!
   n.random();
   rotatev((double) rand() / RAND_MAX * 180.0, n);
   rAxis = 0.0;
//   printDump();

}

/*
myMatrix::myMatrix(vec h)
{
   ptr = newIdentity();
   random();
}
*/

myMatrix::~myMatrix(void)
{
   delete[] ptr;
}

void myMatrix::applyRotate(void)
{	//push and pop must bracket the use of this function
   glMultMatrixf(ptr);
}

void myMatrix::rotatev(const double &angle, const vec &axis)
{
	//cout << "Axis: ";
	//axis.printDump();
   glMatrixMode(GL_MODELVIEW_MATRIX);
   glPushMatrix();
   glLoadMatrixf(ptr);
   glRotated(angle, axis.x, axis.y, axis.z);
   glGetFloatv(GL_MODELVIEW_MATRIX,ptr);
   glPopMatrix();

}


void myMatrix::rotateToward(const double &maxAngle, const vec &target)
{
	vec t = target,
       h = getHead(), l, u(0.0, 0.0, 1.0);
   t.norm();
   double ang = angleBetween(t, u);
   /*
	h.norm();
   vec cross = h * t;
   cross.norm();
	double a = angleBetween(t, h);

	rotatev(fmin(maxAngle, a), cross);*/
	if(ang > 0.05 && ang < 179.95)
   {
      setHead(t);
      l = u * t;
      l.norm();
      setLeft(l);
      u = h * l;
      u.norm();
      setUp(u);
   }

}

/*void myMatrix::rotateToward(const double &maxAngle, const vec &tarvec)
{
   vec target(tarvec);
   target.norm();
   vec head(getHead()), test;
   head.norm();
   vec accel(target - head);
   cout << "Target-head:  " << accel.mag() << endl;
   vec newAxis(head * target);
   newAxis.norm();

   cout << "Head: ";
   head.printDump();
   cout << "Target: ";
   target.printDump();
   cout << "newAxis: ";
   newAxis.printDump();

   double newAngle,
         targetAng = angleBetween(head, target);
   cout << "targetAng: " << targetAng << endl;

   test = getUp();
   test.rotatev(rAxis, head);

   cout << "Angle between Axes: " << angleBetween(test, newAxis) << endl;
	double mult(fmax(0.0, cos(angleBetween(test, newAxis) /180 * M_PI)));
   cout << "cos(Angle between Axes): " << mult << endl;


   if(targetAng < 1.0 || targetAng == 180) //angleBetween(test, newAxis) > MAX_AXIS_CHANGE )
   {
      cout << "Setting new axis to old axis." << endl;

      newAngle = rAxis; //= 'oldAngle'
   }
   else
   {
      if(angleBetween(getLeft(), newAxis) < 90.0)
      {// if desired axis is on boids left
         newAngle = 360.0 - angleBetween(getUp(), newAxis);
         cout << "New Axis is on boids LEFT" << endl;
      }
      else
      {// desired axis is on boids right
         newAngle = angleBetween(getUp(), newAxis);
         cout << "New Axis is on boids RIGHT" << endl;
      }
	cout << newAngle - rAxis << endl;
   cout << "NewAngle :" << newAngle << endl;
   cout << "rAxis    :" << rAxis << endl;

      newAngle = (newAngle < rAxis - 180.0) ? newAngle + 360.0 :
         ((newAngle > rAxis + 180.0) ? newAngle - 360.0 : newAngle);
	}
   //assertion: abs(rAxis - newAngle <= 180.0)
   //assertion: domain(newAngle) = [rAxis - 180.0, rAxis + 180.0]
	cout << newAngle - rAxis << endl;
   cout << "NewAngle :" << newAngle << endl;
   cout << "rAxis    :" << rAxis << endl;
	cout << fmax(-MAX_AXIS_CHANGE, fmin( MAX_AXIS_CHANGE, newAngle - rAxis)) << endl;

   rAxis += fmax(-MAX_AXIS_CHANGE, fmin( MAX_AXIS_CHANGE, newAngle - rAxis));
   cout << "NewAngle :" << newAngle << endl;
   cout << "rAxis    :" << rAxis << endl;

/*   if(newAngle > rAxis - MAX_AXIS_CHANGE)
   {
   	cout << "newAngle is CW from rAxis - Max Change" << endl;

      if(newAngle < rAxis + MAX_AXIS_CHANGE)
      {
         rAxis = newAngle;
   		cout << "Change in axes is less than max change" << endl;
         cout << "rAxis: <- newAngle" << endl;
      }
      else
      {
         rAxis += MAX_AXIS_CHANGE;
         cout << "Turning rAxis CCW by Max Change" << endl;
      }
   }
   else
   {
      rAxis -= MAX_AXIS_CHANGE;
   	cout << "Turning rAxis CW by Max Change" << endl;
   }
   */
   //rAxis = fmod(rAxis, 360);
/*////   rAxis = (rAxis < 0.0) ? rAxis + 360.0 : ((rAxis > 360.0) ? rAxis - 360.0 : rAxis);

   //Assertion: rAxis is within MAX_AXIS_CHANGE of the value of rAxis at the start of this function
   //				 unless it jumped over the 0/360 discontinuity, where rAxis +/- 360 is within the
   //				 required range, which ever is applicable

	newAxis = getUp();
   newAxis.rotatev(rAxis, head);
   cout << "rAxis Angle is now: " << rAxis << endl;

   cout << "targetAng: " << targetAng << endl;
   cout << "final newAxis: ";
   newAxis.printDump();

   //add scaling of targetAng based on how close newAxis is to head * target
   rotatev(fmax(-maxAngle, fmin(maxAngle, targetAng * mult)), newAxis);
}

*/

