/* Arquivo: surface.cpp
   Autores: Gabriel Batista Galli e Vladimir Belinski
   Descrição: o presente arquivo faz parte da resolução do Trabalho II do CCR Computação Gráfica, 2016-2, do curso de
              Ciência da Computação da Universidade Federal da Fronteira Sul - UFFS, o qual consiste na renderização
              de uma imagem utilizando-se NURBS.
              --> surface.cpp é o principal arquivo do trabalho. Nele são realizadas as configurações de visibilidade,
              iluminação, posicionamento de câmera, aplicação de textura, definição e chamada das funções de desenho,
							gerenciamento de teclado, entre outros.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "surface.h"

unsigned int ih = 0, iw = 0;
unsigned char * counter_texture = NULL;
GLuint *textures = new GLuint[MAXTEXTURES];
float ctex_ix = 0.0, ctex_iy = 0.0, ctex_fx = 1.0, ctex_fy = 1.0;
unsigned char* loadBMP_custom(const char*, unsigned int&, unsigned int&);

GLUnurbsObj *nurbs = NULL;
// the position the camera points to
double center_x = 0.0, center_y = 0.0, center_z = 0.0;
// the position of the camera
double cam_x = DEF_CAM_X, cam_y = DEF_CAM_Y, cam_z = DEF_CAM_Z;

GLfloat counter_vertices[8][3] = {
	{ 50.0f, -16.0f, -0.5f}, { 50.0f, 50.0f, -0.5f}, { 50.0f, 50.0f,  0.0f}, { 50.0f, -50.0f,  0.0f},
	{-50.0f, -50.0f,  0.0f}, {-50.0f, 50.0f,  0.0f}, {-50.0f, 50.0f, -0.5f}, {-50.0f, -50.0f, -0.5f}
};

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
	{ {  -0.50, 29.95,  0.0, 1.0 }, { -0.15, 29.95,  0.0, 1.0 }, { 0.15, 29.95,  0.0, 1.0 }, {  0.50, 29.95,  0.0, 1.0 } },
	{ {  -1.00, 29.90,  0.0, 1.0 }, { -0.20, 29.90,  0.0, 1.0 }, { 0.20, 29.90,  0.0, 1.0 }, {  1.00, 29.90,  0.0, 1.0 } },
	{ {  -1.50, 29.50,  0.0, 1.0 }, { -0.25, 29.50,  0.0, 1.0 }, { 0.25, 29.50,  0.0, 1.0 }, {  1.50, 29.50,  0.0, 1.0 } },
	{ {  -2.18, 24.50,  0.0, 1.0 }, { -0.29, 24.50,  0.0, 1.0 }, { 0.29, 24.50,  0.0, 1.0 }, {  2.18, 24.50,  0.0, 1.0 } },
	{ {  -2.78, 21.00,  0.0, 1.0 }, { -0.40, 21.00,  0.0, 1.0 }, { 0.40, 21.00,  0.0, 1.0 }, {  2.78, 21.00,  0.0, 1.0 } },
	{ {  -3.15, 16.75,  0.0, 1.0 }, { -0.65, 16.75,  0.0, 1.0 }, { 0.65, 16.75,  0.0, 1.0 }, {  3.15, 16.75,  0.0, 1.0 } },
	{ {  -2.83, 14.75,  0.0, 1.0 }, { -0.51, 14.75,  0.0, 1.0 }, { 0.51, 14.75,  0.0, 1.0 }, {  2.83, 14.75,  0.0, 1.0 } },
	{ {  -0.40,  14.0,  0.0, 1.0 }, { -0.33,  14.0,  0.0, 1.0 }, { 0.33,  14.0,  0.0, 1.0 }, {  0.40,  14.0,  0.0, 1.0 } },
	{ {  -0.30,  11.5,  1.5, 1.0 }, { -0.30,  11.5,  1.5, 1.0 }, { 0.30,  11.5,  1.5, 1.0 }, {  0.30,  11.5,  1.5, 1.0 } },
	{ {  -0.43,   7.5,  1.0, 1.0 }, { -0.34,   7.5,  1.0, 1.0 }, { 0.34,   7.5,  1.0, 1.0 }, {  0.43,   7.5,  1.0, 1.0 } },
	{ {  -0.60,   3.5,  0.0, 1.0 }, { -0.38,   3.5,  0.0, 1.0 }, { 0.38,   3.5,  0.0, 1.0 }, {  0.60,   3.5,  0.0, 1.0 } },
	{ {  -0.90,   0.0,  0.0, 1.0 }, { -0.47,   0.0,  0.0, 1.0 }, { 0.47,   0.0,  0.0, 1.0 }, {  0.90,   0.0,  0.0, 1.0 } },
	{ {  -0.15,   0.0,  0.0, 1.0 }, { -0.30,   0.0,  0.0, 1.0 }, { 0.30,   0.0,  0.0, 1.0 }, {  0.15,   0.0,  0.0, 1.0 } }
};

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

// puts the points that represent the silhouette of the gourd
// in the first and last columns of its control points matrix
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

// performs a 360-degree rotation of the initial points
// of the gourd so we end up with a whole surface
void init_gourd(void) {
	int i, j;
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

// same as 'init_first_gourd_points', but for the pudding
void init_first_pudding_points(void) {
	int i, j;
	GLfloat pudding_initial_points[][4] = {
		{   0.0,   7.0,  0.0,  1.0},
		{   2.5,   7.0,  0.0,  1.0},
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

// same as 'init_gourd', but for the pudding
void init_pudding(void) {
	int i, j;
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

// all OpenGL-related initialization
void init(void) {
	// generation function of textures
	glGenTextures(1, textures);
	// enabling the use of tetxure
	glEnable(GL_TEXTURE_2D);
	// definig the storage form of pixels in the texture (1 means alignment per byte)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	counter_texture = loadBMP_custom("marble.bmp", iw, ih);
	// association function
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	// definition of the color mix
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// void glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iw, ih, 0, GL_RGB, GL_UNSIGNED_BYTE, counter_texture);
	// GLint gluBuild2DMipmaps(GLenum target, GLint internalFormat, GLsizei width,	GLsizei height, GLenum format, GLenum type, const void *data);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, iw, ih, GL_RGB, GL_UNSIGNED_BYTE, counter_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	// definition of the color interpolation scheme
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// treatment of points outside the texture space
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	delete counter_texture;

	glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	// From documentation:
  // The position is transformed by the modelview matrix when glLight is called (just as if
  // it were a point), and it is stored in eye coordinates. If the w component of the position
  // is 0, the light is treated as a directional source. Diffuse and specular lighting calculations
  // take the light's direction, but not its actual position, into account, and attenuation is
  // disabled. Otherwise, diffuse and specular lighting calculations are based on the actual
  // location of the light in eye coordinates, and attenuation is enabled.
	const GLfloat light0_position[] = {50.0, 20.0, 10.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	const GLfloat light1_position[] = {-20.0, 70.0, -80.0, 1.0};
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

  const GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);

  const GLfloat light_color[] = {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color);

	const GLfloat light1_color[] = {0.4, 0.4, 0.4, 1.0};
	glLightfv(GL_LIGHT0, GL_SPECULAR, light1_color);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_color);

  const GLfloat light_ambient_global[] = {0.2, 0.2, 0.2, 1.0}; // default is 0.2, 0.2, 0.2, 1.0
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient_global);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	// "Enable GL_COLOR_MATERIAL and set glColorMaterial to GL_AMBIENT_AND_DIFFUSE.
	// This means that glMaterial will control the polygon's specular and emission
	// colours and the ambient and diffuse will both be set using glColor."
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	const GLfloat material_specular[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
	const GLfloat material_shininess[] = {128.0}; // 0 to 128. The higher, the "thinner" the "little white glow"
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);

	glShadeModel(GL_SMOOTH); // GL_SMOOTH is the default

	// definition of how perspective must be treated
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

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
	knots_spatula_v[2] = 0.0;
	knots_spatula_v[3] = 0.0;
	knots_spatula_v[4] = 1.0;
	knots_spatula_v[5] = 1.0;
	knots_spatula_v[6] = 1.0;
	knots_spatula_v[7] = 1.0;
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

// in parallelepiped() the coordinates of the parallelepiped are mapped with the coordinates of the texture
void parallelepiped(int a, int b, int c, int d) {
	glTexCoord2f(ctex_fx, ctex_iy);
	glVertex3fv(counter_vertices[a]);
	glTexCoord2f(ctex_fx, ctex_fy);
	glVertex3fv(counter_vertices[b]);
	glTexCoord2f(ctex_ix, ctex_fy);
	glVertex3fv(counter_vertices[c]);
	glTexCoord2f(ctex_fx, ctex_iy);
	glVertex3fv(counter_vertices[a]);
	glTexCoord2f(ctex_ix, ctex_fy);
	glVertex3fv(counter_vertices[c]);
	glTexCoord2f(ctex_ix, ctex_iy);
	glVertex3fv(counter_vertices[d]);
}

// the counter is made by triangles, that together build a parallelepiped. In draw_counter are calculated the normals for the ilumination and called the function parallelepiped() to map the coordinates necessary for the texture
void draw_counter() {
	glPushMatrix();
		glColor3ubv(counter_color);
		glBegin(GL_TRIANGLES);
			glNormal3f(0.0f, -1.0f, 0.0f);
			parallelepiped(0, 3, 4, 7);
			glNormal3f(0.0f, 1.0f, 0.0f);
			parallelepiped(1, 6, 5, 2);
			glNormal3f(1.0f, 0.0f, 0.0f);
			parallelepiped(0, 1, 2, 3);
			glNormal3f(0.0f, 0.0f, 1.0f);
			parallelepiped(3, 2, 5, 4);
			glNormal3f(-1.0f, 0.0f, 0.0f);
			parallelepiped(4, 5, 6, 7);
			glNormal3f(0.0f, 0.0f, -1.0f);
			parallelepiped(1, 0, 7, 6);
		glEnd();
	glPopMatrix();
}

void display(void) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gluLookAt(cam_x, cam_y, cam_z, center_x, center_y, center_z, 0.0, 1.0, 0.0);

	// binding the texture for the counter, positioning and drawing it
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glPushMatrix();
		glTranslatef(0.0f, -1.4f, -20.0f);
		glRotated(-90, 0.0, 1.0, 0.0);
		glRotated(-90, 1.0, 0.0, 0.0);
		draw_counter();
	glPopMatrix();

	// defining GL texture to NULL (standard cleanup)
	glBindTexture(GL_TEXTURE_2D, 0);

	// positioning and drawing the tray
	glPushMatrix();
		glScalef(7.0f, 7.0f, 7.0f);
		draw_tray();
	glPopMatrix();

	// positioning and drawing the spatula
	glPushMatrix();
		glScalef(0.8f, 0.8f, 0.8f);
		glTranslatef(15.0f, -1.2f, 17.0f);
		glRotated(-90, 1.0, 0.0, 0.0);
		glRotated(90, 0.0, 0.0, 1.0);
		draw_spatula();
	glPopMatrix();

	// positioning and drawing the gourd
	glPushMatrix();
		glTranslatef(11.0f, -0.8f, -2.5f);
		glScalef(2.0f, 2.0f, 2.0f);
		draw_gourd();
	glPopMatrix();

	// positioning and drawing the pudding pot
	glPushMatrix();
		glTranslatef(-3.5f, 0.0f, 0.0f);
		glScalef(1.3f, 1.0f, 1.3f);
		draw_pudding();
	glPopMatrix();

	glutSwapBuffers();
}

void reshape(int w, int h) {
	// Avoid division by 0
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0 /* angle of view */, (double) w / h /* aspect ratio */ , 0.1, 150.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void idle(void) {
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
		case 27: // ESC
			exit(0);
		case 'y':
			cam_y = 0.0;
			break;
		case 'Y':
			cam_y = DEF_CAM_Y;
			break;
		case '0':
			cam_x = DEF_CAM_X;
			cam_z = DEF_CAM_Z;
			break;
    case '1':
			cam_x = 30.0;
			cam_z = DEF_CAM_Z;
			break;
    case '2':
			cam_x = 50.0;
			cam_z = 0.0;
			break;
		case '3':
			cam_x = 30.0;
			cam_z = -DEF_CAM_Z;
			break;
		case '4':
			cam_x = DEF_CAM_X;
			cam_z = -DEF_CAM_Z;
			break;
    case '5':
			cam_x = -30.0;
			cam_z = -DEF_CAM_Z;
			break;
    case '6':
			cam_x = -50.0;
			cam_z = 0.0;
			break;
		case '7':
			cam_x = -30.0;
			cam_z = DEF_CAM_Z;
			break;
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
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
