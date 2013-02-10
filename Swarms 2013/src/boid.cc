//boid.cc
#include <stdio.h>
#include <iostream>
#include "math.h"
#include "boid.h"
#include "defines.h"
using std::cout;
using std::endl;
extern boidBox preys;
extern boidBox boids;
extern objectBox objects;


////////////////////////
//       Boid
////////////////////////

boid::boid(void)
:object()  //change this to (double,double,double) or similar
{
   //Eventually, I'll have random placement here, for now,
   // just have one position.
   pos.z = 14.0;  //x,y ar already 0.0
   pos.x = -10.0;
   bound = new boundingSphere(5.0);
   vel = INIT_VEL;
   listID = BOID_ID;
}

boid::boid(vec p)
: object(p)
{
   bound = new boundingSphere(5.0);
   listID = BOID_ID;
}

/*
boid::boid(vec p, vec h)
: object(p, h)
{
   listID = BOID_ID;


}
*/

void boid::updateMe(void)
{

/* // Testing Code
   static vec force(0.0,0.0,-1.0);
	static vec rr(0.0, 0.0, 1.0);
   vec rrr(1.0, 2.0, -3.0);

   force.rotatev(-.3, rr);
   rr.rotatev(.1, rrr);
   force.norm();
   force.printDump();
	double angle = angleBetween(HLU.getHead(), HLU.getHead() + force);
	//cout << "angle to force:" << angle << endl;
*/
   int i;
   //Initial value is a tendancy to hang near the origin
   vec head = HLU.getHead();
   vec force;  // = (pos * (1 / pos.mag()) + head) * -0.1;
   vec preyForce; //stores unweighted Prey Force
   vec avoidForce; //stores unweighted Avoid Force
   vec cohesionForce; //stores unweighted Cohesion Force
   vec matchForce; //stores unweighted Match Velocity Force
   vec to; //vector 'to' the target
   vec z(0, 0, 1);
   float toMag; //precomputed to.mag() (to save on square root calculation.
   float neighbourhoodCount = 0.0;

   //Prey urge
   for(i = 0; i < preys.count(); i++)
   {
      to = preys[i].getPos() - pos; 
      if(to.mag() < 20.0 && angleBetween(to, head) < 120.0)
      {
         preyForce += preys[i].getChaseUrge(*this);
      }   
   }   

   //Object avoid urge
   for(i = 0; i < OBJECT_COUNT; i++)
   {
      to = objects[i].getPos() - pos; 
      avoidForce += objects[i].getAvoidUrge(*this);
   }   
   
   //Urges for fellow boids, avoid, cohesion, match velocity
   for(i = 0; i < boids.count() && neighbourhoodCount < 10; i++)
   {
      to = boids[i].getPos() - pos;
      toMag = to.mag();
      //(toMag > 0.0  == FALSE) when (boids[i] === activeBoid)
      if(toMag < 10.0  && angleBetween(to, head) < 90.0 && toMag > 0.0) 
      {
         avoidForce += boids[i].getAvoidUrge(*this);
         cohesionForce += boids[i].getCohesionUrge(*this);
         matchForce += boids[i].getMatchUrge(*this);
         neighbourhoodCount++;
      }
   }      
   cohesionForce *= (neighbourhoodCount > 0 ? (1 / neighbourhoodCount) : 1);

   force += preyForce * boids.getPreyWeight();
   force += avoidForce * boids.getAvoidWeight();
   force += cohesionForce * boids.getCohesionWeight();
   force += matchForce * boids.getMatchWeight();

   //out-of-bounds
   if(pos.z < -40.0)
   {
      force = z; //* 0.2 * (pos.z + 40.0);
   }
   else
   {
      toMag = pos.mag();
      if(toMag > 90.0)
      force = pos * (1 / toMag) * -1;
      //force += pos * (1 / toMag) * -0.1 * (toMag - 80);
   }   
   
   force.norm();
	force *= vel *.2;

   vec newHead = HLU.getHead() * vel + force;
   HLU.rotateToward(180.0, newHead);
   pos += newHead;

   //Comment out this line for constant velocity
   vel = fmax(MIN_BOID_VEL, fmin(MAX_BOID_VEL, newHead.mag()));
}

vec boid::getMatchUrge(const boid& activeBoid)
{
   return getHead() * vel - activeBoid.getHead() * activeBoid.vel;
}

vec boid::getCohesionUrge(const boid& activeBoid)
{
   return (pos - activeBoid.getPos())* MAX_BOID_VEL -
           activeBoid.getHead() * activeBoid.vel;
}

vec boid::getChaseUrge(const boid& activeBoid)
{
   //return the difference between current velocity vector,
   // and the desired velocity vector
   return (pos - activeBoid.getPos())* MAX_BOID_VEL -
           activeBoid.getHead() * activeBoid.vel;
}

////////////////////////
//        Prey
////////////////////////
prey::prey(void)
: boid()  //change this to (double,double,double) or similar
{
   bound = new boundingSphere(2.0);
   vel = INIT_VEL;
   listID = PREY_ID;
}

prey::prey(vec p)
: boid(p)
{
   bound = new boundingSphere(2.0);
   listID = PREY_ID;
}


void prey::updateMe(void)
{
	//vec force = (pos * (1 / pos.mag()) + HLU.getHead()) * -1.0;
   //vec newHead;
      int i;
   //Initial value is a tendancy to hang near the origin
   vec head = HLU.getHead();
   vec force;  // = (pos * (1 / pos.mag()) + head) * -0.1;
   vec avoidForce; //stores unweighted Avoid Force
   vec matchForce; //stores unweighted Match Velocity Force
   vec to; //vector 'to' the target
   vec z(0, 0, 1);
   float toMag; //precomputed to.mag() (to save on square root calculation.
   float neighbourhoodCount = 0.0;

   //Prey dodge other prey urge
   for(i = 0; i < preys.count(); i++)
   {
      to = preys[i].getPos() - pos; 
      toMag = to.mag();
      if(toMag < 20.0 && toMag > 0.0 && angleBetween(to, head) < 120.0)
      {
         avoidForce += preys[i].getAvoidUrge(*this);
         
      }   
   }   

   //Object avoid urge
   for(i = 0; i < OBJECT_COUNT; i++)
   {
      to = objects[i].getPos() - pos; 
      avoidForce += objects[i].getAvoidUrge(*this);
   }   
   
   //Urges to evade predator boids
   for(i = 0; i < boids.count(); i++)
   {
      to = boids[i].getPos() - pos;
      toMag = to.mag();
      
      if(toMag < 10.0 && toMag > 0.0) 
      {
         avoidForce += boids[i].getAvoidUrge(*this);
      }
   }      

   force += avoidForce;    // * urgeWeights(AVOID);

   //out-of-bounds
   if(pos.z < -40.0)
   {
      force = z; // * 0.2 * (pos.z + 40.0);
   }
   else
   {
      toMag = pos.mag();
      if(toMag > 90.0)
      force = pos * (1 / toMag) * -1;
      //force += pos * (1 / toMag) * -0.2 * (toMag - 90)*(toMag - 90);
   }   

   force.norm();
	force *= vel *.2;

   vec newHead = HLU.getHead() * vel + force;
   HLU.rotateToward(180.0, newHead);
   pos += newHead;

   //Comment out this line for constant velocity
   vel = fmax(MIN_PREY_VEL, fmin(MAX_PREY_VEL, newHead.mag()));
}


////////////////////////
//     Boid Box
////////////////////////

boidBox::boidBox(int type, unsigned int newCount)
{
	if(type == BOID_ID)
   {
   	list = new boid[newCount];
   }
   else
   {
   	list = new prey[newCount];
   }
	boidCount = newCount;
	urgeWeights[PREY] = 0.5;
	urgeWeights[AVOID] = 0.5;
	urgeWeights[COHESION] = 0.5;
	urgeWeights[MATCH] = 0.5;
}
boidBox::~boidBox(void)
{
   delete[] list;  
}   

boid& boidBox::operator[](int i)
{
   return list[i < boidCount ? (i > 0 ? i : 0) : boidCount];
}

unsigned int boidBox::count(void)
{
   return boidCount;
}

//redimension array
void boidBox::reDim(int type, unsigned int newCount)
{
   delete[] list;
	if(type == BOID_ID)
   {
   	list = new boid[newCount];
   }
   else
   {
   	list = new prey[newCount];
   }
   boidCount = newCount;
}


