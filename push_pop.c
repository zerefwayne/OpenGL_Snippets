#include <GL/glut.h> 
#include <stdio.h>
#include <math.h>


/*  Poor man's approximation of PI */
#define PI 3.1415926535898
/*  Macro for sin & cos in degrees */
#define Cos(th) cos(PI/180*(th))
#define Sin(th) sin(PI/180*(th))
/*  D degrees of rotation */
#define DEF_D 5

/*  Globals */
double dim=5.0; /* dimension of orthogonal box */
char *windowName = "OpenGL screenscasts 10: Push and Pop Matrix and complex objects";
int windowWidth=500;
int windowHeight=450;

/*  Various global state */
int toggleAxes = 1;   /* toggle axes on and off */
int toggleValues = 1; /* toggle values on and off */
int toggleMode = 0; /* projection mode */
int th = 340;   /* azimuth of view angle */
int ph = 30;    /* elevation of view angle */
int fov = 55;   /* field of view for perspective */
int asp = 1;    /* aspect ratio */

/*  Cube vertices */
GLfloat vertA[3] = { 0.5, 0.5, 0.5};
GLfloat vertB[3] = {-0.5, 0.5, 0.5};
GLfloat vertC[3] = {-0.5,-0.5, 0.5};
GLfloat vertD[3] = { 0.5,-0.5, 0.5};
GLfloat vertE[3] = { 0.5, 0.5,-0.5};
GLfloat vertF[3] = {-0.5, 0.5,-0.5};
GLfloat vertG[3] = {-0.5,-0.5,-0.5};
GLfloat vertH[3] = { 0.5,-0.5,-0.5};

/*
 * project()
 * ------
 * Sets the projection
 */
void project() 
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (toggleMode) {
    /* perspective */
    gluPerspective(fov,asp,dim/4,4*dim);
  }
  else {
    /* orthogonal projection*/
    glOrtho(-dim*asp,+dim*asp, -dim,+dim, -dim,+dim);
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/*
 * setEye()
 * ------
 * Set the eye position
 */
void setEye()
{
  if (toggleMode) {
    double Ex = -2*dim*Sin(th)*Cos(ph);
    double Ey = +2*dim        *Sin(ph);
    double Ez = +2*dim*Cos(th)*Cos(ph);
    /* camera/eye position, aim of camera lens, up-vector */
    gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
  }
  /*  Orthogonal - set world orientation */
  else {
    glRotatef(ph,1,0,0);
    glRotatef(th,0,1,0);
  }
}

/*
 * drawAxes()
 * ------
 * Draw the axes
 */
void drawAxes() 
{
  if (toggleAxes) {
    /*  Length of axes */
    double len = 2.0;
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(len,0,0);
    glVertex3d(0,0,0);
    glVertex3d(0,len,0);
    glVertex3d(0,0,0);
    glVertex3d(0,0,len);
    glEnd();
    /*  Label axes */
    glRasterPos3d(len,0,0);
    // print("X");
    glRasterPos3d(0,len,0);
    // print("Y");
    glRasterPos3d(0,0,len);
    // print("Z");
  }
}

/*
 *  drawValues()
 *  ------
 *  Draw the values in the lower left corner
 */
void drawValues()
{
  if (toggleValues) {
    glColor3f(0.8,0.8,0.8);
    printf("View Angle (th, ph) =(%d, %d)\n", th,ph);
    printf("5,25,""Projection mode =(%s)\n", toggleMode?"Perspective":"Orthogonal");
  }
}

/*
 *  cube
 *  ------
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
void cube(double x,double y,double z,
	  double dx,double dy,double dz,
	  double th)
{
  glPushMatrix();
  /*  Transform cube */
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  /* Cube */
  glBegin(GL_QUADS);
  /* front => ABCD yellow */
  glColor3f(1.0,1.0,0.0);
  glVertex3fv(vertA);
  glVertex3fv(vertB);
  glVertex3fv(vertC);
  glVertex3fv(vertD);
  /* back => FEHG red */
  glColor3f(1.0,0.0,0.0);
  glVertex3fv(vertF);
  glVertex3fv(vertE);
  glVertex3fv(vertH);
  glVertex3fv(vertG);
  /* right => EADH green */
  glColor3f(0.0,1.0,0.0);
  glVertex3fv(vertE);
  glVertex3fv(vertA);
  glVertex3fv(vertD);
  glVertex3fv(vertH);
  /* left => BFGC blue */
  glColor3f(0.0,0.0,1.0);
  glVertex3fv(vertB);
  glVertex3fv(vertF);
  glVertex3fv(vertG);
  glVertex3fv(vertC);
  /* top => EFBA turquoise */
  glColor3f(0.0,1.0,1.0);
  glVertex3fv(vertE);
  glVertex3fv(vertF);
  glVertex3fv(vertB);
  glVertex3fv(vertA);
  /* bottom => DCGH pink */
  glColor3f(1.0,0.0,1.0);
  glVertex3fv(vertD);
  glVertex3fv(vertC);
  glVertex3fv(vertG);
  glVertex3fv(vertH);
  glEnd();

  glPopMatrix();
}

/*
 *  cone
 *  ------
 *  Draws a cone
 *     at (x,y,z)
 *     with radius r and height h
 *     with 360/deg sides
 */
void cone(double x,double y,double z,
	  double r,double h,int deg)
{
  int k;
  glPushMatrix();

  /*  Transformation */
  glTranslated(x,y,z);
  glScaled(r,h,r);
  glRotated(-90,1,0,0);

  /* sides */
  glBegin(GL_TRIANGLES);
  for (k=0;k<=360;k+=deg){
    glColor3f(0.0,0.0,1.0);
    glVertex3f(0,0,1);
    glColor3f(0.0,1.0,1.0);
    glVertex3f(Cos(k),Sin(k),0);
    glColor3f(1.0,0.0,1.0);
    glVertex3f(Cos(k+deg),Sin(k+deg),0);
  }
  glEnd();

  /* bottom circle */ 
  /* rotate back */
  glRotated(90,1,0,0);
  glBegin(GL_TRIANGLES);
  glColor3f(1.0,1.0,0.0);
  for (k=0;k<=360;k+=deg) {
    glVertex3f(0,0,0);
    glVertex3f(Cos(k),0,Sin(k));
    glVertex3f(Cos(k+deg),0,Sin(k+deg));
  }
  glEnd();

  glPopMatrix();
}

/*
 *  spike
 *  ------
 *  Draw a spike
 *     at (x, y, z)
 *     radius r, height h, with 360/deg sides
 *     rotated ox around the x axis
 *     rotated oy around the y axis
 *     rotated oz around the z axis
 */
void spike(double x, double y, double z,
	   double r,double h,int deg,
	   double ox,double oy,double oz)
{
  glPushMatrix();
  glRotated(oz,0,0,1);
  glRotated(oy,0,1,0);
  glRotated(ox,1,0,0);

  cone(x,y,z, r,h,deg);
  glPopMatrix();
}

/*
 *  towers
 *  ------
 *  Draw a tower
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
void tower(double x,double y,double z,
	   double dx,double dy,double dz,
	   double th)
{
  glPushMatrix();
  /*  Transformation  */
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  cube(0,1.5,0, 1,3,1, 0);
  cube(0,3.5,0, 2,1,2, 45);
  spike(0,1,-3.5, 0.5,1, 90, 90,0,0);
  spike(0,1,3.5, 0.5,1, 90, -90,0,0);
  spike(-3.5,1,0, 0.5,1, 90, 0,0,-90);
  spike(3.5,1,0, 0.5,1, 90, 0,0,90);

  glPopMatrix();
}

/*
 *  display()
 *  ------
 *  Display the scene
 */
void display()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();

  /* setup functions */
  setEye();

  /* draw */
  drawAxes();
  drawValues();

  /* magic here */
  /*
  cube(1,0,1, 1,1,1, 0);
  cube(-1,0,1, 1,1,1, 0);
  cone(0,1,0, 1,1,DEF_D);
  cone(0,-1,0, 1,1,90);
  */

  /* a 'tower' */
  /*
  cube(0,1.5,0, 1,3,1, 0);
  cube(0,3.5,0, 2,1,2, 45);
  spike(0,1,-3.5, 0.5,1, 90, 90,0,0);
  spike(0,1,3.5, 0.5,1, 90, -90,0,0);
  spike(-3.5,1,0, 0.5,1, 90, 0,0,-90);
  spike(3.5,1,0, 0.5,1, 90, 0,0,90);
  */
  tower(0,0,0, 1,1,1, 0);
  tower(4,0,0, 1,1,1.5, 30);

  glFlush();
  glutSwapBuffers();
}

/*
 *  reshape()
 *  ------
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
  asp = (height>0) ? (double)width/height : 1;
  glViewport(0,0, width,height);
  project();
}

/*
 *  windowKey()
 *  ------
 *  GLUT calls this routine when a non-special key is pressed
 */
void windowKey(unsigned char key,int x,int y)
{
  /*  Exit on ESC */
  if (key == 27) exit(0);
  else if (key == 'a' || key == 'A') toggleAxes = 1-toggleAxes;
  else if (key == 'v' || key == 'V') toggleValues = 1-toggleValues;
  else if (key == 'm' || key == 'M') toggleMode = 1-toggleMode;
  /*  Change field of view angle */
  else if (key == '-' && key>1) fov--;
  else if (key == '+' && key<179) fov++;
  /*  Change dimensions */
  else if (key == 'D') dim += 0.1;
  else if (key == 'd' && dim>1) dim -= 0.1;

  project();
  glutPostRedisplay();
}

/*
 *  windowSpecial()
 *  ------
 *  GLUT calls this routine when an arrow key is pressed
 */
void windowSpecial(int key,int x,int y)
{
  /*  Right arrow key - increase azimuth by 5 degrees */
  if (key == GLUT_KEY_RIGHT) th += 5;
  /*  Left arrow key - decrease azimuth by 5 degrees */
  else if (key == GLUT_KEY_LEFT) th -= 5;
  /*  Up arrow key - increase elevation by 5 degrees */
  else if (key == GLUT_KEY_UP) ph += 5;
  /*  Down arrow key - decrease elevation by 5 degrees */
  else if (key == GLUT_KEY_DOWN) ph -= 5;

  /*  Keep angles to +/-360 degrees */
  th %= 360;
  ph %= 360;

  project();
  glutPostRedisplay();
}

/*
 *  windowMenu
 *  ------
 *  Window menu is the same as the keyboard clicks
 */
void windowMenu(int value)
{
  windowKey((unsigned char)value, 0, 0);
}

/*
 *  main()
 *  ----
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(windowWidth,windowHeight);
  glutCreateWindow(windowName);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(windowKey);
  glutSpecialFunc(windowSpecial);

  glutCreateMenu(windowMenu);
  glutAddMenuEntry("Toggle Axes [a]",'a');
  glutAddMenuEntry("Toggle Values [v]",'v');
  glutAddMenuEntry("Toggle Mode [m]",'m');
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutMainLoop();
  return 0;
}