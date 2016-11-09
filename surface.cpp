#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "surface.h"

// the position the camera points to
double center_x = 0.0, center_y = 0.0, center_z = 0.0;
// the position of the camera
double cam_x = DEF_CAM_X, cam_y = DEF_CAM_Y, cam_z = DEF_CAM_Z;

bool showPoints = false;
GLUnurbsObj *nurbs = NULL;
GLfloat knots_tray_u[CTRLPOINTS_TRAY_U + 4], knots_tray_v[CTRLPOINTS_TRAY_V + 4];

GLfloat ctlpoints_curve[CTRLPOINTS_TRAY_U][CTRLPOINTS_TRAY_V][4] = {
	{ {-1.5, 0.0,  -1.4}, {  -0.5,   0.0,  -1.5}, {  0.5,   0.0,  -1.5}, {1.5, 0.0,  -1.4} },
	{ {-2.5, 0.0, -1.35}, { -3.00, -0.25, -1.40}, { 3.00, -0.25, -1.40}, {2.5, 0.0, -1.35} },
	{ {-2.5, 0.0,  -0.5}, { -2.75, -0.25, -1.40}, { 2.75, -0.25, -1.40}, {2.5, 0.0,  -0.5} },
	{ {-2.5, 0.0,   0.5}, { -2.75, -0.25,  1.30}, { 2.75, -0.25,  1.40}, {2.5, 0.0,   0.5} },
	{ {-2.5, 0.0,  1.35}, { -3.00, -0.25,  1.40}, { 3.00, -0.25,  1.40}, {2.5, 0.0,  1.35} },
	{ {-1.5, 0.0,   1.4}, {  -0.5,   0.0,   1.5}, {  0.5,   0.0,   1.5}, {1.5, 0.0,   1.4} }
};

// circle
// GLfloat ctlpoints_curve[CTRLPOINTS_TRAY_U][4] = {
// 	{  0.0, -4.0,  0.0,              1.0 },
// 	{ -4.0, -4.0,  0.0,  sin(M_PI / 4.0) },
// 	{ -4.0,  0.0,  0.0,              1.0 },
// 	{ -4.0,  4.0,  0.0,  sin(M_PI / 4.0) },
// 	{  0.0,  4.0,  0.0,              1.0 },
// 	{  4.0,  4.0,  0.0,  sin(M_PI / 4.0) },
// 	{  4.0,  0.0,  0.0,              1.0 },
// 	{  4.0, -4.0,  0.0,  sin(M_PI / 4.0) },
// 	{  0.0, -4.0,  0.0,              1.0 }
// };

void nurbsError(GLenum errorCode) {
	const GLubyte *estring = NULL;
	estring = gluErrorString(errorCode);
	fprintf (stderr, "NURBS Error: %s\n", estring);
	exit(-1);
}

void init(void) {
	glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	// From documentation:
  // The position is transformed by the modelview matrix when glLight is called (just as if
  // it were a point), and it is stored in eye coordinates. If the w component of the position
  // is 0, the light is treated as a directional source. Diffuse and specular lighting calculations
  // take the light's direction, but not its actual position, into account, and attenuation is
  // disabled. Otherwise, diffuse and specular lighting calculations are based on the actual
  // location of the light in eye coordinates, and attenuation is enabled.
  const GLfloat light0_position[] = {25.0, 25.0, 25.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  const GLfloat light1_position[] = {-25.0, -25.0, -25.0, 1.0};
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  const GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  const GLfloat light0_color[] = {1.0, 1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_color);
  const GLfloat light1_color[] = {0.0, 0.5, 1.0, 1.0};
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_color);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light1_color);
  const GLfloat light_ambient_global[] = {0.2, 0.2, 0.2, 1.0}; // default is 0.2, 0.2, 0.2, 1.0
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient_global);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  // glEnable(GL_LIGHT1);

	// "Enable GL_COLOR_MATERIAL and set glColorMaterial to GL_AMBIENT_AND_DIFFUSE.
	// This means that glMaterial will control the polygon's specular and emission
	// colours and the ambient and diffuse will both be set using glColor."
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	const GLfloat material_specular[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
	const GLfloat material_shininess[] = {128.0}; // 0 to 128. The higher, the "thinner" the "little white glow"
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);

	glShadeModel(GL_SMOOTH); // GL_SMOOTH is the default

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	nurbs = gluNewNurbsRenderer();
	/* value should be set to be either GLU_NURBS_RENDERER or GLU_NURBS_TESSELLATOR.
	 * When set to GLU_NURBS_RENDERER, NURBS objects are tessellated into OpenGL
	 * primitives and sent to the pipeline for rendering. When set to GLU_NURBS_TESSELLATOR,
	 * NURBS objects are tessellated into OpenGL primitives but the vertices, normals, colors,
	 * and/or textures are retrieved back through a callback interface (see gluNurbsCallback).
	 * This allows the user to cache the tessellated results for further processing.
	 * The initial value is GLU_NURBS_RENDERER.
	 **/
	gluNurbsProperty(nurbs, GLU_NURBS_MODE, GLU_NURBS_RENDERER);
	/* When set to GLU_PATH_LENGTH, the surface is rendered so that the maximum length, in pixels,
	 * of the edges of the tessellation polygons is no greater than what is specified by GLU_SAMPLING_TOLERANCE.
	 * The initial value is GLU_PATH_LENGTH.
	 **/
	gluNurbsProperty(nurbs, GLU_SAMPLING_METHOD, GLU_PATH_LENGTH);
	/* Specifies the maximum length, in pixels or in object space length unit, to use when the
	 * sampling method is set to GLU_PATH_LENGTH or GLU_OBJECT_PATH_LENGTH. The NURBS code is
	 * conservative when rendering a curve or surface, so the actual length can be somewhat shorter.
	 * The initial value is 50.0 pixels.
	 **/
	gluNurbsProperty(nurbs, GLU_SAMPLING_TOLERANCE, 25.0);
	/* value can be set to GLU_OUTLINE_POLYGON, GLU_FILL, or GLU_OUTLINE_PATCH.
	 * When GLU_NURBS_MODE is set to be GLU_NURBS_RENDERER, value defines how a
	 * & NURBS surface should be rendered. When value is set to GLU_FILL, the surface
	 * is rendered as a set of polygons. When value is set to GLU_OUTLINE_POLYGON,
	 * the NURBS library draws only the outlines of the polygons created by tessellation.
	 * When value is set to GLU_OUTLINE_PATCH just the outlines of patches and trim
	 * curves defined by the user are drawn.
	 * The initial value is GLU_FILL.
	 **/
	gluNurbsProperty(nurbs, GLU_DISPLAY_MODE, GLU_FILL);
	gluNurbsCallback(nurbs, GLU_ERROR, (GLvoid (*)()) nurbsError);

	knots_tray_u[0] =  0.0;
	knots_tray_u[1] =  0.0;
	knots_tray_u[2] =  0.0;
	knots_tray_u[3] =  0.0;
	knots_tray_u[4] =  0.5;
	knots_tray_u[5] =  0.5;
	knots_tray_u[6] =  1.0;
	knots_tray_u[7] =  1.0;
	knots_tray_u[8] =  1.0;
	knots_tray_u[9] =  1.0;

	knots_tray_v[0] = 0.0;
	knots_tray_v[1] = 0.0;
	knots_tray_v[2] = 0.0;
	knots_tray_v[3] = 0.0;
	knots_tray_v[4] = 1.0;
	knots_tray_v[5] = 1.0;
	knots_tray_v[6] = 1.0;
	knots_tray_v[7] = 1.0;
}

void draw_control_points_tray(void) {
	int i, j;
	glPushMatrix();
		glColor3ubv(tray_color);
		glBegin(GL_POINTS);
			for (i = 0; i < CTRLPOINTS_TRAY_U; i++)
				for (j = 0; j < CTRLPOINTS_TRAY_V; j++)
					glVertex3f(ctlpoints_curve[i][j][0], ctlpoints_curve[i][j][1], ctlpoints_curve[i][j][2]);
		glEnd();
	glPopMatrix();
}

void draw_control_points(void) {
	glPushMatrix();
		glPointSize(5.0);
		glDisable(GL_LIGHTING);
		draw_control_points_tray();
		glEnable(GL_LIGHTING);
	glPopMatrix();
}

void draw_tray(void) {
	glPushMatrix();
		glColor3ubv(tray_color);
		gluBeginSurface(nurbs);
			/* void gluNurbsSurface(GLUnurbs* nurb, GLint sKnotCount, GLfloat* sKnots, GLint tKnotCount, GLfloat* tKnots, GLint sStride,
			 * GLint tStride, GLfloat* control, GLint sOrder, GLint tOrder, GLenum type);
			 * sKnotCount:	Specifies the number of knots in the parametric u direction.
			 * sKnots:			Specifies an array of sKnotCount nondecreasing knot values in the parametric u direction.
			 * tKnotCount: 	Specifies the number of knots in the parametric v direction.
			 * tKnots:			Specifies an array of tKnotCount nondecreasing knot values in the parametric v direction.
			 * sStride:			Specifies the offset (as a number of single-precision floating-point values) between
			 * 							successive control points in the parametric u direction in control.
			 * tStride:			Specifies the offset (in single-precision floating-point values) between successive
			 * 							control points in the parametric v direction in control.
			 * control:			Specifies an array containing control points for the NURBS surface. The offsets between
			 * 							successive control points in the parametric u and v directions are given by sStride and tStride.
			 * sOrder:			Specifies the order of the NURBS surface in the parametric u direction. The order is one
			 * 							more than the degree, hence a surface that is cubic in u has a u order of 4.
			 * tOrder:			Specifies the order of the NURBS surface in the parametric v direction. The order is one
			 * 							more than the degree, hence a surface that is cubic in v has a v order of 4.
			 * type:				Specifies type of the surface. type can be any of the valid two-dimensional evaluator
			 * 							types (such as GLU_MAP2_VERTEX_3 or GLU_MAP2_COLOR_4).
			 * Note that a gluNurbsSurface with sKnotCount knots in the u direction and tKnotCount knots in the v direction
			 * with orders sOrder and tOrder must have (sKnotCount - sOrder) times (tKnotCount - tOrder) control points.
			 **/
			gluNurbsSurface(nurbs, CTRLPOINTS_TRAY_U + 4, knots_tray_u, CTRLPOINTS_TRAY_V + 4, knots_tray_v, CTRLPOINTS_TRAY_V * 4, 4, &ctlpoints_curve[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
		gluEndSurface(nurbs);
	glPopMatrix();
}

void display(void) {
	int i, j;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gluLookAt(cam_x, cam_y, cam_z, center_x, center_y, center_z, 0.0, 1.0, 0.0);

	draw_tray();

	if (showPoints) draw_control_points();
	glutSwapBuffers();
}

void reshape(int w, int h) {
	// Avoid division by 0
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0 /* angle of view */, (double) w / h /* aspect ratio */ , 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void idle(void) {
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
  double camx = cam_x;
  double camy = cam_y;
  double camz = cam_z;
  switch (key) {
		case 27:
			exit(0);
    case 'w':
      cam_y = camy * COS_ONE_STEP + camz * SIN_ONE_STEP;
      cam_z = -camy * SIN_ONE_STEP + camz * COS_ONE_STEP;
      break;
    case 'a':
      cam_x = camx * COS_ONE_STEP - camz * SIN_ONE_STEP;
      cam_z = camx * SIN_ONE_STEP + camz * COS_ONE_STEP;
      break;
    case 's':
      cam_y = camy * COS_ONE_STEP - camz * SIN_ONE_STEP;
      cam_z = camy * SIN_ONE_STEP + camz * COS_ONE_STEP;
      break;
    case 'd':
      cam_x = camx * COS_ONE_STEP + camz * SIN_ONE_STEP;
      cam_z = -camx * SIN_ONE_STEP + camz * COS_ONE_STEP;
      break;
    case 'e':
      cam_x = COS_ONE_STEP * camx + SIN_ONE_STEP * camy;
      cam_y = -SIN_ONE_STEP * camx + COS_ONE_STEP * camy;
      break;
    case 'q':
      cam_x = COS_ONE_STEP * camx - SIN_ONE_STEP * camy;
      cam_y = SIN_ONE_STEP * camx + COS_ONE_STEP * camy;
      break;
    case 'W':
      cam_y += PAN_STEP;
      center_y += PAN_STEP;
      break;
    case 'A':
      cam_x -= PAN_STEP;
      center_x -= PAN_STEP;
      break;
    case 'S':
      cam_y -= PAN_STEP;
      center_y -= PAN_STEP;
      break;
    case 'D':
      cam_x += PAN_STEP;
      center_x += PAN_STEP;
      break;
    case '=':
      if (cam_z > 0) cam_z -= PAN_STEP;
      else if (cam_z < 0) cam_z += PAN_STEP;
      break;
    case '-':
      if (cam_z >= 0) cam_z += PAN_STEP;
      else if (cam_z < 0) cam_z -= PAN_STEP;
      break;
    case 'c':
      cam_x = DEF_CAM_X;
      cam_y = DEF_CAM_Y;
      cam_z = DEF_CAM_Z;
      break;
    case 'C':
      center_x = center_y = center_z = 0.0;
      break;
		case 'p':
		case 'P':
			showPoints = !showPoints;
  }
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(720, 720);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	// glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
