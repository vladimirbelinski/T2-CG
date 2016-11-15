#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "surface.h"
using namespace std;

bool showPoints = false;
GLUnurbsObj *nurbs = NULL;
// the position the camera points to
double center_x = 0.0, center_y = 0.0, center_z = 0.0;
// the position of the camera
double cam_x = DEF_CAM_X, cam_y = DEF_CAM_Y, cam_z = DEF_CAM_Z;

GLfloat knots_tray_u[CTRLPOINTS_TRAY_U + 4], knots_tray_v[CTRLPOINTS_TRAY_V + 4];
GLfloat ctrlpoints_tray[CTRLPOINTS_TRAY_U][CTRLPOINTS_TRAY_V][4] = {
	{ {-1.5, 0.0,  -1.4, 1.0}, {  -0.5,   0.0,  -1.5, 1.0}, {  0.5,   0.0,  -1.5, 1.0}, {1.5, 0.0,  -1.4, 1.0} },
	{ {-2.5, 0.0, -1.35, 1.0}, { -3.00, -0.25, -1.40, 1.0}, { 3.00, -0.25, -1.40, 1.0}, {2.5, 0.0, -1.35, 1.0} },
	{ {-2.5, 0.0,  -0.5, 1.0}, { -2.75, -0.25, -1.40, 1.0}, { 2.75, -0.25, -1.40, 1.0}, {2.5, 0.0,  -0.5, 1.0} },
	{ {-2.5, 0.0,   0.5, 1.0}, { -2.75, -0.25,  1.30, 1.0}, { 2.75, -0.25,  1.40, 1.0}, {2.5, 0.0,   0.5, 1.0} },
	{ {-2.5, 0.0,  1.35, 1.0}, { -3.00, -0.25,  1.40, 1.0}, { 3.00, -0.25,  1.40, 1.0}, {2.5, 0.0,  1.35, 1.0} },
	{ {-1.5, 0.0,   1.4, 1.0}, {  -0.5,   0.0,   1.5, 1.0}, {  0.5,   0.0,   1.5, 1.0}, {1.5, 0.0,   1.4, 1.0} }
};

GLfloat knots_spatula_u[CTRLPOINTS_SPATULA_U + 4], knots_spatula_v[CTRLPOINTS_SPATULA_V + 4];
GLfloat ctrlpoints_spatula[CTRLPOINTS_SPATULA_U][CTRLPOINTS_SPATULA_V][4] = {
	{ {  0.00,   0.0,  0.0, 1.0 }, {  0.00,   0.0,  0.0, 1.0 }, { 0.00,   0.0,  0.0, 1.0 }, { 0.00,   0.0,  0.0, 1.0 } },
	{ { -0.50,   0.0,  0.0, 1.0 }, { -0.17,   0.0,  0.0, 1.0 }, { 0.17,   0.0,  0.0, 1.0 }, { 0.50,   0.0,  0.0, 1.0 } },
	{ { -0.25,   3.5,  0.0, 1.0 }, { -0.08,   3.5,  0.0, 1.0 }, { 0.08,   3.5,  0.0, 1.0 }, { 0.25,   3.5,  0.0, 1.0 } },
	{ { -0.13,   7.5,  1.0, 1.0 }, { -0.04,   7.5,  1.0, 1.0 }, { 0.04,   7.5,  1.0, 1.0 }, { 0.13,   7.5,  1.0, 1.0 } },
	{ {  0.00,  11.5,  1.5, 1.0 }, {  0.00,  11.5,  1.5, 1.0 }, { 0.00,  11.5,  1.5, 1.0 }, { 0.00,  11.5,  1.5, 1.0 } },
	{ { -0.10,  14.0,  0.0, 1.0 }, { -0.03,  14.0,  0.0, 1.0 }, { 0.03,  14.0,  0.0, 1.0 }, { 0.10,  14.0,  0.0, 1.0 } },
	{ { -0.63, 14.75,  0.0, 1.0 }, { -0.21, 14.75,  0.0, 1.0 }, { 0.21, 14.75,  0.0, 1.0 }, { 0.63, 14.75,  0.0, 1.0 } },
	{ { -1.05, 16.75,  0.0, 1.0 }, { -0.35, 16.75,  0.0, 1.0 }, { 0.35, 16.75,  0.0, 1.0 }, { 1.05, 16.75,  0.0, 1.0 } },
	{ { -0.78,  21.0,  0.0, 1.0 }, { -0.26,  21.0,  0.0, 1.0 }, { 0.26,  21.0,  0.0, 1.0 }, { 0.78,  21.0,  0.0, 1.0 } },
	{ { -0.58,  24.5,  0.0, 1.0 }, { -0.19,  24.5,  0.0, 1.0 }, { 0.19,  24.5,  0.0, 1.0 }, { 0.58,  24.5,  0.0, 1.0 } },
	{ { -0.30,  29.5,  0.0, 1.0 }, { -0.10,  29.5,  0.0, 1.0 }, { 0.10,  29.5,  0.0, 1.0 }, { 0.30,  29.5,  0.0, 1.0 } },
	{ { -0.15,  29.0,  0.0, 1.0 }, { -0.05,  29.0,  0.0, 1.0 }, { 0.05,  29.0,  0.0, 1.0 }, { 0.15,  29.0,  0.0, 1.0 } },
	{ {  0.00,  29.0,  0.0, 1.0 }, {  0.00,  29.0,  0.0, 1.0 }, { 0.00,  29.0,  0.0, 1.0 }, { 0.00,  29.0,  0.0, 1.0 } }
};

// { {  0.0,   0.0,  0.0, 1.0} },
// { {  2.0,   0.0,  0.0, 1.0} },
// { {  1.0,   3.5,  0.0, 1.0} },
// { {  0.5,   7.5,  1.0, 1.0} },
// { {  0.0,  11.5,  1.5, 1.0} },
// { {  0.4,  14.0,  0.0, 1.0} },
// { {  2.5, 14.75,  0.0, 1.0} },
// { {  4.2, 16.75,  0.0, 1.0} },
// { {  3.1,  21.0,  0.0, 1.0} },
// { {  2.3,  24.5,  0.0, 1.0} },
// { {  1.2,  29.5,  0.0, 1.0} },
// { {  0.6,  29.0,  0.0, 1.0} },
// { {  0.0,  29.0,  0.0, 1.0} }

// GLfloat ctrlpoints_gourd[CTRLPOINTS_GOURD_U][CTRLPOINTS_GOURD_V][4];
// GLfloat knots_gourd_u[CTRLPOINTS_TRAY_U + 4], knots_gourd_v[CTRLPOINTS_TRAY_V + 4];
GLfloat ctrlpoints_gourd[CTRLPOINTS_GOURD_U][CTRLPOINTS_GOURD_V][4],
	ctrlpoints_pudding[CTRLPOINTS_PUDDING_U][CTRLPOINTS_PUDDING_V][4];
GLfloat knots_gourd_u[CTRLPOINTS_GOURD_U + 4], knots_gourd_v[CTRLPOINTS_GOURD_V + 4],
	knots_pudding_u[CTRLPOINTS_PUDDING_U + 4], knots_pudding_v[CTRLPOINTS_PUDDING_V + 4];

void nurbsError(GLenum errorCode) {
	const GLubyte *estring = NULL;
	estring = gluErrorString(errorCode);
	fprintf (stderr, "NURBS Error: %s\n", estring);
	exit(-1);
}

void init_first_gourd_points(void) {
	int i, j;
	GLfloat gourd_initial_points[][4] = {
		{   0.0,   0.0,  0.0,  1.0},
		{   2.0,   0.0,  0.0,  1.0},
		{   2.5,   0.0,  0.0,  1.0},
		{   0.0,   0.5,  0.0,  1.0},
		{   1.0,   1.0,  0.0,  1.0},
		{   2.0,  1.75,  0.0,  1.0},
		{   2.0,  4.75,  0.0,  1.0},
		{  0.75,  4.75,  0.0,  1.0},
		{   2.5,  6.25,  0.0,  1.0}
	};
	for (i = 0; i < CTRLPOINTS_GOURD_U; i++) {
		for (j = 0; j < 4; j++) {
			ctrlpoints_gourd[i][0][j] = gourd_initial_points[i][j];
			ctrlpoints_gourd[i][CTRLPOINTS_GOURD_V - 1][j] = gourd_initial_points[i][j];
		}
	}
}

void init_gourd(void) {
	int i, j, k = 0;
	init_first_gourd_points();
	for (i = 1; i < CTRLPOINTS_GOURD_V - 1; i++) {
		for (j = 0; j < CTRLPOINTS_GOURD_U; j++) {
			// cam_x = camx * COS_ONE_STEP + camz * SIN_ONE_STEP;
			ctrlpoints_gourd[j][i][0] = ctrlpoints_gourd[j][i - 1][0] * COS_FOURTY_FIVE + ctrlpoints_gourd[j][i - 1][2] * SIN_FOURTY_FIVE;

			ctrlpoints_gourd[j][i][1] = ctrlpoints_gourd[j][i - 1][1];

			// cam_z = -camx * SIN_ONE_STEP + camz * COS_ONE_STEP;
			ctrlpoints_gourd[j][i][2] = -ctrlpoints_gourd[j][i - 1][0] * SIN_FOURTY_FIVE + ctrlpoints_gourd[j][i - 1][2] * COS_FOURTY_FIVE;

			ctrlpoints_gourd[j][i][3] = ctrlpoints_gourd[j][i - 1][3];
		}
	}
}

void init_first_pudding_points(void) {
	int i, j;
	GLfloat pudding_initial_points[][4] = {
		{   0.0,   5.0,  0.0,  1.0},
		{   2.5,   5.0,  0.0,  1.0},
		{   2.5,   0.0,  0.0,  1.0},
		{   3.5,   0.0,  0.0,  1.0},
		{   6.5,   0.0,  0.0,  1.0},
		{   9.0,   0.0,  0.0,  1.0},
		{   9.0,   8.5,  0.0,  1.0}
	};
	for (i = 0; i < CTRLPOINTS_PUDDING_U; i++) {
		for (j = 0; j < 4; j++) {
			ctrlpoints_pudding[i][0][j] = pudding_initial_points[i][j];
			ctrlpoints_pudding[i][CTRLPOINTS_PUDDING_V - 1][j] = pudding_initial_points[i][j];
		}
	}
}

void init_pudding(void) {
	int i, j, k = 0;
	init_first_pudding_points();
	for (i = 1; i < CTRLPOINTS_PUDDING_V - 1; i++) {
		for (j = 0; j < CTRLPOINTS_PUDDING_U; j++) {
			// cam_x = camx * COS_ONE_STEP + camz * SIN_ONE_STEP;
			ctrlpoints_pudding[j][i][0] = ctrlpoints_pudding[j][i - 1][0] * COS_FOURTY_FIVE + ctrlpoints_pudding[j][i - 1][2] * SIN_FOURTY_FIVE;

			ctrlpoints_pudding[j][i][1] = ctrlpoints_pudding[j][i - 1][1];

			// cam_z = -camx * SIN_ONE_STEP + camz * COS_ONE_STEP;
			ctrlpoints_pudding[j][i][2] = -ctrlpoints_pudding[j][i - 1][0] * SIN_FOURTY_FIVE + ctrlpoints_pudding[j][i - 1][2] * COS_FOURTY_FIVE;

			ctrlpoints_pudding[j][i][3] = ctrlpoints_pudding[j][i - 1][3];
		}
	}
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
  const GLfloat light1_position[] = {-25.0, 25.0, 25.0, 1.0};
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	const GLfloat light2_position[] = {25.0, 25.0, -25.0, 1.0};
  glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	const GLfloat light3_position[] = {-25.0, 25.0, -25.0, 1.0};
  glLightfv(GL_LIGHT3, GL_POSITION, light3_position);

  const GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);

  const GLfloat light_color[] = {1.0, 1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_color);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_color);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light_color);
  glLightfv(GL_LIGHT3, GL_DIFFUSE, light_color);
  glLightfv(GL_LIGHT3, GL_SPECULAR, light_color);

  const GLfloat light_ambient_global[] = {0.2, 0.2, 0.2, 1.0}; // default is 0.2, 0.2, 0.2, 1.0
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient_global);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  // glEnable(GL_LIGHT1);
	// glEnable(GL_LIGHT2);
	// glEnable(GL_LIGHT3);

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

	glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	init_gourd();
	knots_gourd_u[0] =  0.0;
	knots_gourd_u[1] =  0.0;
	knots_gourd_u[2] =  0.0;
	knots_gourd_u[3] =  0.0;
	knots_gourd_u[4] =  0.25;
	knots_gourd_u[5] =  0.5;
	knots_gourd_u[6] =  0.5;
	knots_gourd_u[7] =  0.5;
	knots_gourd_u[8] =  0.75;
	knots_gourd_u[9] =  1.0;
	knots_gourd_u[10] =  1.0;
	knots_gourd_u[11] =  1.0;
	knots_gourd_u[12] =  1.0;

	knots_gourd_v[0] = 0.0;
	knots_gourd_v[1] = 0.0;
	knots_gourd_v[2] = 0.0;
	knots_gourd_v[3] = 0.0;
	knots_gourd_v[4] = 0.25;
	knots_gourd_v[5] = 0.5;
	knots_gourd_v[6] = 0.5;
	knots_gourd_v[7] = 0.5;
	knots_gourd_v[8] = 0.75;
	knots_gourd_v[9] = 1.0;
	knots_gourd_v[10] = 1.0;
	knots_gourd_v[11] = 1.0;
	knots_gourd_v[12] = 1.0;

	init_pudding();
	knots_pudding_u[0] =  0.0;
	knots_pudding_u[1] =  0.0;
	knots_pudding_u[2] =  0.0;
	knots_pudding_u[3] =  0.0;
	knots_pudding_u[4] =  0.25;
	knots_pudding_u[5] =  0.5;
	knots_pudding_u[6] =  0.75;
	knots_pudding_u[7] =  1.0;
	knots_pudding_u[8] =  1.0;
	knots_pudding_u[9] =  1.0;
	knots_pudding_u[10] =  1.0;

	knots_pudding_v[0] = 0.0;
	knots_pudding_v[1] = 0.0;
	knots_pudding_v[2] = 0.0;
	knots_pudding_v[3] = 0.0;
	knots_pudding_v[4] = 0.25;
	knots_pudding_v[5] = 0.5;
	knots_pudding_v[6] = 0.5;
	knots_pudding_v[7] = 0.5;
	knots_pudding_v[8] = 0.75;
	knots_pudding_v[9] = 1.0;
	knots_pudding_v[10] = 1.0;
	knots_pudding_v[11] = 1.0;
	knots_pudding_v[12] = 1.0;

	knots_spatula_u[0] =  0.0;
	knots_spatula_u[1] =  0.0;
	knots_spatula_u[2] =  0.0;
	knots_spatula_u[3] =  0.0;
	knots_spatula_u[4] =  0.1;
	knots_spatula_u[5] =  0.2;
	knots_spatula_u[6] =  0.3;
	knots_spatula_u[7] =  0.4;
	knots_spatula_u[8] =  0.5;
	knots_spatula_u[9] =  0.6;
	knots_spatula_u[10] =  0.7;
	knots_spatula_u[11] =  0.8;
	knots_spatula_u[12] =  0.9;
	knots_spatula_u[13] =  1.0;
	knots_spatula_u[14] =  1.0;
	knots_spatula_u[15] =  1.0;
	knots_spatula_u[16] =  1.0;

	knots_spatula_v[0] = 0.0;
	knots_spatula_v[1] = 0.0;
	knots_spatula_v[2] = 0.25;
	knots_spatula_v[3] = 0.5;
	knots_spatula_v[4] = 0.5;
	knots_spatula_v[5] = 0.75;
	knots_spatula_v[6] = 1.0;
	knots_spatula_v[7] = 1.0;
}

void draw_control_points_tray(void) {
	int i, j;
	glPushMatrix();
		glColor3ubv(tray_color);
		glBegin(GL_POINTS);
			for (i = 0; i < CTRLPOINTS_TRAY_U; i++)
				for (j = 0; j < CTRLPOINTS_TRAY_V; j++)
					glVertex3f(ctrlpoints_tray[i][j][0], ctrlpoints_tray[i][j][1], ctrlpoints_tray[i][j][2]);
		glEnd();
	glPopMatrix();
}

void draw_control_points_gourd(void) {
	int i, j;
	glPushMatrix();
		glColor4fv(gourd_color);
		glBegin(GL_POINTS);
			for (i = 0; i < CTRLPOINTS_GOURD_U; i++)
				for (j = 0; j < CTRLPOINTS_GOURD_V; j++)
					glVertex3f(ctrlpoints_gourd[i][j][0], ctrlpoints_gourd[i][j][1], ctrlpoints_gourd[i][j][2]);
		glEnd();
	glPopMatrix();
}

void draw_control_points_pudding(void) {
	int i, j;
	glPushMatrix();
		glColor4fv(pudding_color);
		glBegin(GL_POINTS);
			for (i = 0; i < CTRLPOINTS_PUDDING_U; i++)
				for (j = 0; j < CTRLPOINTS_PUDDING_V; j++)
					glVertex3f(ctrlpoints_pudding[i][j][0], ctrlpoints_pudding[i][j][1], ctrlpoints_pudding[i][j][2]);
		glEnd();
	glPopMatrix();
}

void draw_control_points(void) {
	glPushMatrix();
		glPointSize(5.0);
		glDisable(GL_LIGHTING);
		draw_control_points_tray();
		draw_control_points_gourd();
		draw_control_points_pudding();
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
			gluNurbsSurface(nurbs, CTRLPOINTS_TRAY_U + 4, knots_tray_u, CTRLPOINTS_TRAY_V + 4, knots_tray_v, CTRLPOINTS_TRAY_V * 4, 4, &ctrlpoints_tray[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
		gluEndSurface(nurbs);
	glPopMatrix();
}

void draw_gourd(void) {
	glPushMatrix();
		glColor4fv(gourd_color);
		gluBeginSurface(nurbs);
			gluNurbsSurface(nurbs, CTRLPOINTS_GOURD_U + 4, knots_gourd_u, CTRLPOINTS_GOURD_V + 4, knots_gourd_v, CTRLPOINTS_GOURD_V * 4, 4, &ctrlpoints_gourd[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
		gluEndSurface(nurbs);
	glPopMatrix();
}

void draw_pudding(void) {
	glPushMatrix();
		glColor4fv(pudding_color);
		gluBeginSurface(nurbs);
			gluNurbsSurface(nurbs, CTRLPOINTS_PUDDING_U + 4, knots_pudding_u, CTRLPOINTS_PUDDING_V + 4, knots_pudding_v, CTRLPOINTS_PUDDING_V * 4, 4, &ctrlpoints_pudding[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
		gluEndSurface(nurbs);
	glPopMatrix();
}

void draw_spatula(void) {
	glPushMatrix();
		glColor3ubv(spatula_color);
		gluBeginSurface(nurbs);
			gluNurbsSurface(nurbs, CTRLPOINTS_SPATULA_U + 4, knots_spatula_u, CTRLPOINTS_SPATULA_V + 4, knots_spatula_v, CTRLPOINTS_SPATULA_V * 4, 4, &ctrlpoints_spatula[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
		gluEndSurface(nurbs);
	glPopMatrix();
}

void display(void) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gluLookAt(cam_x, cam_y, cam_z, center_x, center_y, center_z, 0.0, 1.0, 0.0);

	// glPushMatrix();
	// 	glScaled(6.0, 6.0, 6.0);
	// 	draw_tray();
	// glPopMatrix();
	//
	// glPushMatrix();
	// 	glTranslated(20.0, 0.0, 0.0);
	// 	draw_gourd();
	// glPopMatrix();
	//
	// draw_pudding();

	draw_spatula();

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
		case 27: // ESC
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
	glutCreateWindow("NURBS");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	// glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
