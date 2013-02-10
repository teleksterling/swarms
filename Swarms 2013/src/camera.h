//camera.h
//Camera object class - contain all viewing information of camera

#ifndef __camera_h__
#define __camera_h__

#include "data.h"

class cam : public myMatrix
{
 private:
   vec pos; 
   float fov;
   float aspectRatio;

 public:
   int viewmode;

   float angleRight;
   float angleUp;

   vec getEye(void) const;
   vec getLookat(void) const;
   //vec getUp(void) const;

   void setEye(const double &x, const double &y, const double &z);
   void setLookat(const double &x, const double &y, const double &z);
   void setUp(const double &x, const double &y, const double &z);
   void setEye(const vec &v);
   void setLookat(const vec &v);
   //void setUp(const vec &v);


   void aspect(float a);
   float aspect(void);
   

   void zoomIn(void);
   void zoomOut(void);
   void zoom(float ang);
   float zoom(void);

   //void setCamera(void);  //To make GL calls for camera setup each frame
};

#endif
