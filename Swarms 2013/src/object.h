//object.h
//General Object Class to exist in boid world

#ifndef __object_h__
#define __object_h__

#include "data.h"


class boundingObject
{
 public:
      //Intersect Parameters: c - position of object to bound,
      //                      p - position of agent
      //                      h - heading of agent
   virtual vec doIntersect(const vec& c, const vec& p, const vec& h) = 0;
   virtual void drawMe(void) = 0;
};



class boundingSphere : public boundingObject
{
 private:
   double radius;
   

 public:
   boundingSphere(void);
   boundingSphere(const double& rad);
   
   vec doIntersect(const vec& c, const vec& p, const vec& h);
   void setRadius(float rad);
   void drawMe(void);
};



class object
{
 protected:
   vec pos;
   mtx HLU;
   boundingObject* bound;

 public:
   object(void);
   object(vec p);
//   object(vec p, vec h);
   

   double vel;
   int listID;

   void random(void);
   void set(const vec& p, const vec& h, const vec& l, const vec& u);
   virtual void set(float rad, float x, float y, float z) { };

   vec getAvoidUrge(const object& activeBoid);

   double getX(void) const;
   double getY(void) const;
   double getZ(void) const;
   double getr(void) const;
   vec getrAxis(void) const;
   vec getPos(void) const;
   vec getHead(void) const;
   vec getUp(void) const;

   //return vector representing direction to avoid this object
   //virtual vec getAvoidUrge(const object& activeBoid);

   virtual void drawMe(void);
   virtual void updateMe(void);

};



class sphere : public object
{
 private:
   float radius;
 public:
    void drawMe(void);
    void set(float rad, float x, float y, float z);
    float getRadius(void) {return radius;};
};

class objectBox
{
 private:
	object* list;
	unsigned int count;
    
 public:
   objectBox(int type, unsigned int newCount);
   ~objectBox(void);
    
	object& operator[](int i);

};



#endif

