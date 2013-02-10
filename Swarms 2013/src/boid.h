//boid.h
//Boid Class to exist in boid world

#ifndef __boid_h__
#define __boid_h__

#include "object.h"
#include "defines.h"

//For the index of urgeWeights
enum {PREY = 0, AVOID, COHESION, MATCH};


class boid : public object
{
 private:
 public:
   boid(void);
   boid(vec p);
   //boid(vec p, vec h);



   virtual void updateMe(void);


   //Gets vectors to avoid/match velocity/cohere with this boid
   // The 'activeBoid' is the one currently deciding its path.
   vec getMatchUrge(const boid& activeBoid);
   vec getCohesionUrge(const boid& activeBoid);
   vec getChaseUrge(const boid& activeboid);

};

class prey : public boid
{
 public:
   prey(void);
   prey(vec p);

   void updateMe(void);

   // The 'activeBoid' is the one currently deciding its path.
};


#endif

class boidBox
{
 private:
	boid* list;
	unsigned int boidCount;
   float urgeWeights[4];
    
 public:
   boidBox(int type, unsigned int newCount);
   ~boidBox(void);
    
	boid& operator[](int i);
	unsigned int count(void);
	void reDim(int type, unsigned int newCount);

   void setWeights(float p, float a, float c, float m); //'p'rey, 'a'void, 'c'ohesion, 'm'atch
   void setPreyWeight(float x) {urgeWeights[PREY] = x;};
   void setAvoidWeight(float x) {urgeWeights[AVOID] = x;};
   void setCohesionWeight(float x) {urgeWeights[COHESION] = x;};
   void setMatchWeight(float x) {urgeWeights[MATCH] = x;};

   float getPreyWeight(void) {return urgeWeights[PREY];};
   float getAvoidWeight(void) {return urgeWeights[AVOID];};
   float getCohesionWeight(void) {return urgeWeights[COHESION];};
   float getMatchWeight(void) {return urgeWeights[MATCH];};

};
