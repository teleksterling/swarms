/* Copyright 2004 Rick Williams
*/


#include "test.h"
#include "world.h"
#include "boid.h"
#include "defines.h"
#include "camera.h"
using std::cout;
using std::endl;



Cworld world;
boidBox boids(BOID_ID, BOID_COUNT);
boidBox preys(PREY_ID, PREY_COUNT);
objectBox objects(WIRESPHERE_ID, OBJECT_COUNT);

cam camera;

//boid 0 (zero) is positioned manually by these values:
// 'p'osition and HLU
// All other boids are positioned randomly on a sphere centered at the origin
vec p(10, 0, 1);
vec h(1, 0, 0);
vec l(0, 1, 0);
vec u(0, 0, 1);

void display(void)
{
	int i;
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );



   /* set matrix mode to modelview */
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   setCamera();

   world.drawMe();
   for(i = 0; i < boids.count(); i++)
	{
   	boids[i].drawMe();
 	}

   for(i = 0; i < preys.count(); i++)
	{
   	preys[i].drawMe();
 	}

   for(i = 0; i < OBJECT_COUNT; i++)
	{
   	objects[i].drawMe();
 	}

   //start = glutGet(GLUT_ELASPED_TIME);
   //end = glutGet(GLUT_ELAPSED_TIME);
   //fps = 1000.0 / (end-start)
   glutSwapBuffers();
}

/*
* setCamera
*
* sets the camera to be looking from the +ive y axis looking toward the origin
*/
void setCamera()
{
	boid &myBoid = boids[0];

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(camera.zoom(), camera.aspect(), 1.0, 1000.0);

   /* set matrix mode to modelview */
   glMatrixMode(GL_MODELVIEW);

   double dist = -5.0; //units in front of boid for camera
   vec eye, look, up;

   switch (camera.viewmode)
   {
   case 1:
   //	To watch the origin
      gluLookAt(200.0, 0.0, 30.0, 		/* eye */
            0.0, 0.0, 0.0,	/* lookat */
            0.0, 0.0, 1.0); 	/* up */
      glRotatef(camera.angleUp, 0.0, -1.0, 0.0);
      glRotatef(camera.angleRight, 0.0, 0.0, 1.0);
   break;
   case 2:
      //	To watch boid
      gluLookAt(-30.0, 30.0, 10.0, /* eye position */
            myBoid.getX(), myBoid.getY(), myBoid.getZ(), /* lookat point */
            0.0, 0.0, 1.0); /* up is in +ive z */
   break;
   case 3:
      //to Follow boid
      eye =  myBoid.getPos() + (myBoid.getHead() - myBoid.getUp() * 0.1)* dist;
      look = myBoid.getPos();
      up = myBoid.getUp();
      gluLookAt(eye.x, eye.y, eye.z, /* eye position */
                look.x, look.y, look.z,  /* lookat point */
                up.x, up.y, up.z); /* up is in +ive z */
   break;
   case 4:
      //to watch looking down rotation axis
      eye =  myBoid.getPos() + myBoid.getrAxis()* -dist;
      look = myBoid.getPos();
      up = myBoid.getHead();
      gluLookAt(eye.x, eye.y, eye.z, /* eye position */
                look.x, look.y, look.z,  /* lookat point */
                up.x, up.y, up.z); /* up is in +ive z */
   break;
   case 5:
      //to follow boid from above
      eye =  myBoid.getPos() + myBoid.getUp() * -dist;
      look = myBoid.getPos();
      up = myBoid.getHead();
      gluLookAt(eye.x, eye.y, eye.z, /* eye position */
                look.x, look.y, look.z,  /* lookat point */
                up.x, up.y, up.z); /* up is in +ive z */
   break;
   }


}

void myReshape(int w, int h)
{
   glViewport(0, 0, w, h);
   camera.aspect((double) w / h);

}

void myKey(unsigned char key, int x, int y)
{
	int i, j;
	float urgeVal;

   switch (key)
   {
   case 'q':
   case 'Q':
      exit(0);
      break;

   case 'r':
   case 'R':
		for(i = 0; i < boids.count(); i++)
      {
	   //boids[i].set(p, h, l, u);
			boids[i].random();
		}
	   boids[0].set(p, h, l, u);
		for(i = 0; i < preys.count(); i++)
      {
	   //boids[i].set(p, h, l, u);
			preys[i].random();
		}
      break;

   case 'u': //prey
      urgeVal = boids.getPreyWeight() - 0.05;
      boids.setPreyWeight((urgeVal >= 0.0) ? ((urgeVal <= 1.0) ? urgeVal: 1.0): 0.0);
      cout << "Prey Weight now " << boids.getPreyWeight() << endl;
      break;

   case 'U':
      urgeVal = boids.getPreyWeight() + 0.05;
      boids.setPreyWeight((urgeVal >= 0.0) ? ((urgeVal <= 1.0) ? urgeVal: 1.0): 0.0);
      cout << "Prey Weight now " << boids.getPreyWeight() << endl;
      break;

   case 'i': //avoid
      urgeVal = boids.getAvoidWeight() - 0.05;
      boids.setAvoidWeight((urgeVal >= 0.0) ? ((urgeVal <= 1.0) ? urgeVal: 1.0): 0.0);
      cout << "Avoid Weight now " << boids.getAvoidWeight() << endl;
      break;

   case 'I':
      urgeVal = boids.getAvoidWeight() + 0.05;
      boids.setAvoidWeight((urgeVal >= 0.0) ? ((urgeVal <= 1.0) ? urgeVal: 1.0): 0.0);
      cout << "Avoid Weight now " << boids.getAvoidWeight() << endl;
      break;

   case 'o': //cohesion
      urgeVal = boids.getCohesionWeight() - 0.05;
      boids.setCohesionWeight((urgeVal >= 0.0) ? ((urgeVal <= 1.0) ? urgeVal: 1.0): 0.0);
      cout << "Cohesion Weight now " << boids.getCohesionWeight() << endl;
      break;

   case 'O':  
      urgeVal = boids.getCohesionWeight() + 0.05;
      boids.setCohesionWeight((urgeVal >= 0.0) ? ((urgeVal <= 1.0) ? urgeVal: 1.0): 0.0);
      cout << "Cohesion Weight now " << boids.getCohesionWeight() << endl;
      break;

   case 'p':  //match
      urgeVal = boids.getMatchWeight() - 0.05;
      boids.setMatchWeight((urgeVal >= 0.0) ? ((urgeVal <= 1.0) ? urgeVal: 1.0): 0.0);
      cout << "Match Weight now " << boids.getMatchWeight() << endl;
      break;

   case 'P':
      urgeVal = boids.getMatchWeight() + 0.05;
      boids.setMatchWeight((urgeVal >= 0.0) ? ((urgeVal <= 1.0) ? urgeVal: 1.0): 0.0);
      cout << "Match Weight now " << boids.getMatchWeight() << endl;
      break;

   case 's':  //reduce timer delay
      world.speed -= (world.speed > 1) ? 1 : 0;
      cout << "Timer Delay now " << world.speed << "ms" << endl;
      break;

   case 'S': //increase timer delay
      world.speed++;
      cout << "Timer Delay now " << world.speed << "ms" << endl;
      break;

   case ' ':
      world.flipStatus();
      break;

   case ',':
      camera.angleRight -= 3;
      break;

   case '.':
      camera.angleRight += 3;
      break;

   case '/':
      world.step();
      break;

   case 'f':
      glutFullScreen();
      break;

   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
      camera.viewmode = key - 48; //asciicode for 1-5 is 49-53
      cout << "Camera Mode now in viewmode " << camera.viewmode << endl;
      break;

   case '+':
      camera.zoomIn();
      cout << "Zoom In: " << camera.zoom() << endl;
      break;

   case '-':
      camera.zoomOut();
      cout << "Zoom Out: " << camera.zoom() << endl;
      break;

   default:
      printf("Unknown keyboard command \'%c\'.\n", key);
      break;
   }
//	printf("Key: \'%c\'.\n", key);
}


/*
* myMouse
*///glMatrixMode(GL_MODELVIEW_MATRIX);
//glPushMatrix;
//glLoadMatrixd(mtx x);
//glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
//glGetDoublev(GL_MODELVIEW_MATRIX, GLdouble *params);

//void myMouse(int btn, int state, int x, int y)
/*{

   if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN);
   if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN);
   if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN);
*/
/*	glutPostRedisplay();
}*/

/*
* myTimerFunc
*
* Updates all boids, and other things...
*/
void myTimerFunc(int value)
{
	int i;
//   cout << endl;
   if(world.getStatus() != PAUSED)
   {
   	for(i = 0; i < boids.count(); i++)
   	{
//   	   if(i% 5 == 0)
//            cout << endl;
//         cout << i << ": ";

   		boids[i].updateMe();
   	}
   	for(i = 0; i < preys.count(); i++)
   	{
   		preys[i].updateMe();
   	}
   	if(world.getStatus() == STEP)
   	{
         world.pause();
      }   
  	}  	
   glutPostRedisplay();

   glutTimerFunc(world.speed,myTimerFunc,0);
}

/*
* userMenu
*
* callback, called by the GLUT when the user selects a menu option.
* This function currently contains "magic numbers" in that the menu numbers
* and their functions are hard coded. A better option would be to use macros
* or have a data structure that relates menu number, name and function...
*/
void userMenu(int value)
{
   switch(value)
   {
   case 1:
		camera.viewmode = 1;
   break;
   case 2:
		camera.viewmode = 2;
   break;
   case 3:
		camera.viewmode = 3;
   break;
   case 4:
      camera.viewmode = 4;
   break;
   case 5:
      camera.viewmode = 5;
   break;
   default:
   fprintf(stderr, "Unknown menu option %d\n", value);
   break;
   }
}
void buildLists(void)
{
   int i;
   
   glNewList(WORLD_ID,GL_COMPILE);
   glMatrixMode(GL_MODELVIEW);
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
     glColor3f(0.0, 0.5, 0.0);
     glNormal3f(0.000000, 0.000000, 1.000000);
     for(i = 0; i < 360; i += 10)
     {
        glVertex3f(86.60 * cos(i * 0.0174533),  86.60 * sin(i * 0.0174533), -50.0);
     }
	 glEnd();

    glColor3f(0.3, 0.3, 0.5);
	 glutWireSphere(100.0, 20, 16);
   glPopMatrix();
   glPopAttrib();
   glEndList();

   glNewList(BOID_ID,GL_COMPILE);
   glMatrixMode(GL_MODELVIEW);
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glPushMatrix();
      glRotatef(-90.0, 0.0, 0.0, 1.0);
      glBegin(GL_TRIANGLE_FAN);
         glColor3f(0.0, 0.0, 1.0);
         glNormal3f(0.000000, 0.000000, -1.000000);
         glVertex3f(0.0, -1.00, -0.50);
         glColor3f(1.0, 1.0, 0.0);
         glNormal3f(-0.704361, 0.088045, -0.704361);
         glVertex3f(0.00, 1.00, 0.00);
         glColor3f(0.0, 0.0, 1.0);
         glNormal3f(-0.707107, -0.707107, 0.000000);
         glVertex3f(-0.25, -1.00, 0.00);
         glColor3f(0.0, 0.0, 1.0);
         glNormal3f(0.707107, -0.707107, 0.000000);
         glVertex3f(0.25, -1.00, 0.00);
         glColor3f(1.0, 1.0, 0.0);
         glNormal3f(0.704361, 0.088045, -0.704361);
         glVertex3f(0.00, 1.00, 0.00);
      glEnd();
      glBegin(GL_TRIANGLES);
         glColor3f(1.0, 1.0, 0.0);
         glNormal3f(0.000000, 0.000000, 1.000000);
         glVertex3f(0.00, 1.00, 0.00);
         glColor3f(1.0, 0.0, 0.0);
         glNormal3f(0.000000, 0.000000, 1.000000);
         glVertex3f(-0.75, -1.00, 0.00);
         glColor3f(1.0, 0.0, 0.0);
         glNormal3f(0.000000, 0.000000, 1.000000);
         glVertex3f(0.75, -1.00, 0.00);
         glColor3f(1.0, 1.0, 0.0);
         glNormal3f(0.000000, 0.000000, -1.000000);
         glVertex3f(0.00, 1.00, 0.00);
         glColor3f(1.0, 0.0, 0.0);
         glNormal3f(0.000000, 0.000000, -1.000000);
         glVertex3f(0.75, -1.00, 0.00);
         glColor3f(1.0, 0.0, 0.0);
         glNormal3f(0.000000, 0.000000, -1.000000);
         glVertex3f(-0.75, -1.00, 0.00);
      glEnd();
   glPopMatrix();
   glPopAttrib();
   glEndList();


   glNewList(PREY_ID,GL_COMPILE);
   glMatrixMode(GL_MODELVIEW);
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glPushMatrix();
      glRotatef(-90.0, 0.0, 0.0, 1.0);
      glBegin(GL_TRIANGLE_FAN);
         glColor3f(0.0, 1.0, 0.0);
         glNormal3f(0.000000, 0.000000, -1.000000);
         glVertex3f(0.0, -1.00, -0.50);
         glColor3f(0.0, 1.0, 0.0);
         glNormal3f(-0.704361, 0.088045, -0.704361);
         glVertex3f(0.00, 1.00, 0.00);
         glColor3f(0.0, 1.0, 0.0);
         glNormal3f(-0.707107, -0.707107, 0.000000);
         glVertex3f(-0.25, -1.00, 0.00);
         glColor3f(0.0, 1.0, 0.0);
         glNormal3f(0.707107, -0.707107, 0.000000);
         glVertex3f(0.25, -1.00, 0.00);
         glColor3f(0.0, 1.0, 0.0);
         glNormal3f(0.704361, 0.088045, -0.704361);
         glVertex3f(0.00, 1.00, 0.00);
      glEnd();
      glBegin(GL_TRIANGLES);
         glColor3f(0.0, 1.0, 0.0);
         glNormal3f(0.000000, 0.000000, 1.000000);
         glVertex3f(0.00, 1.00, 0.00);
         glColor3f(0.0, 0.5, 0.0);
         glNormal3f(0.000000, 0.000000, 1.000000);
         glVertex3f(-0.75, -1.00, 0.00);
         glColor3f(0.0, 0.5, 0.0);
         glNormal3f(0.000000, 0.000000, 1.000000);
         glVertex3f(0.75, -1.00, 0.00);
         glColor3f(0.0, 1.0, 0.0);
         glNormal3f(0.000000, 0.000000, -1.000000);
         glVertex3f(0.00, 1.00, 0.00);
         glColor3f(0.0, 0.5, 0.0);
         glNormal3f(0.000000, 0.000000, -1.000000);
         glVertex3f(0.75, -1.00, 0.00);
         glColor3f(0.0, 0.5, 0.0);
         glNormal3f(0.000000, 0.000000, -1.000000);
         glVertex3f(-0.75, -1.00, 0.00);
      glEnd();
   glPopMatrix();
   glPopAttrib();
   glEndList();
   
   glNewList(WIRESPHERE_ID,GL_COMPILE);
   glMatrixMode(GL_MODELVIEW);
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glPushMatrix();
      glutWireSphere(1.0, 4, 4);
   glPopMatrix();
   glPopAttrib();
   glEndList();

}

void initWorldnBoid(void)
{
	int i;
   glEnable(GL_DEPTH_TEST);
   srand( (unsigned)time( NULL ) );

   world.listID = WORLD_ID;
   camera.viewmode = 1;
   camera.zoom(50.0);
   camera.aspect(1.0);
   camera.angleRight = 45;
   camera.angleUp = 10;
   

	for(i = 0; i < boids.count(); i++)
	{
		boids[i].random();
	}
	boids[0].set(p, h, l, u);

   for(i = 0; i < preys.count(); i++)
	{
		preys[i].random();
	}

   for(i = 0; i < OBJECT_COUNT; i++)
	{
		objects[i].set(8.0 + 4 * i,
                     (60.0 + 2 * i) * cos(i * 90.0 * 0.017453),
                     (60.0 + 2 * i) * sin(i * 90.0 * 0.017453),
                     i * 10.0 - 30.0);
	}

	buildLists();
}



/*
* main
*
* Initialization and sets graphics callbacks
*
*/
int main(int argc, char **argv)
{

   /* glutInit MUST be called before any other GLUT/OpenGL calls */
   glutInit(&argc, argv);

   /* set double buffering, z buffering */
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(800, 800);
   glutCreateWindow("Predator Swarms \"When Killer Planes Attack!\"- Rick Williams, ID# 92570224");

   /* set callback functions */
   glutReshapeFunc(myReshape);
   glutDisplayFunc(display);
// glutIdleFunc(myIdleFunc);
   glutTimerFunc(10, myTimerFunc, 0);
   glutKeyboardFunc(myKey);

  /* set up right button menu */
   glutCreateMenu(userMenu);
   glutAddMenuEntry("Watch Origin", 1);
   glutAddMenuEntry("Watch Boid", 2);
   glutAddMenuEntry("Follow Boid from behind", 3);
   glutAddMenuEntry("Follow boid from rotation axis", 4);
   glutAddMenuEntry("Follow boid from up vector", 5);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
   

// Processing command line parameters
   for (int i = 1; i < argc; i++)
   {
      if(argv[i][0] == '-')
      {
         switch (argv[i][1])
         {
            case 'n':  //set boid count
               cout << "Boid Count = " << atoi(argv[i]+2)<< endl;
               boids.reDim(BOID_ID, atoi(argv[i]+2));
               break;
            case 'p':  //set prey count
               cout << "Prey Count = " << atoi(argv[i]+2)<< endl;
               preys.reDim(PREY_ID, atoi(argv[i]+2));
               break;
         }
      }
   }
   
/* Processing command line parameters
   for (i = 1; i < argc; i++)
   {
      if (!strcmp("-", argv[i]))
      {
         ;
      }
      else if (!strcmp("-", argv[i]))
      {
         ;
      }
      else if (!strcmp("-", argv[i]))
      {
         ;
      }
   }
*/

   initWorldnBoid();


//	userSettings();
   glutMainLoop();

   return 0;
}
