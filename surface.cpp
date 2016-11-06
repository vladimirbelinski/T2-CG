#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

float angx, angy;
int showPoints = 0;
GLUnurbsObj *theNurb = NULL;
GLfloat ctlpoints[4][4][3] = {
	{ { -0.6, 1.0, -1.5 }, { -0.3, 1.0, -1.6 }, { 0.3, 1.0, -1.6 }, { 0.6, 1.0, -1.5 } },
	{ { -1.5, 1.0, -0.5 }, { -0.5, 5.0,  0.0 }, { 0.5, 5.0,  0.0 }, { 1.5, 1.0, -0.5 } },
	{ { -1.5, 1.0,  0.5 }, { -0.5, 5.0,  0.0 }, { 0.5, 5.0,  0.0 }, { 1.5, 1.0,  0.5 } },
	{ { -0.7, 1.0,  1.5 }, { -0.5, 3.0,  1.7 }, { 0.5, 3.0,  1.7 }, { 0.7, 1.0,  1.5 } }
};

void init_surface(void) {
	int u, v;
	for (u = 0; u < 4; u++) {
		for (v = 0; v < 4; v++) {
			ctlpoints[u][v][0] = 2.0 * ((GLfloat) u - 1.5);
			ctlpoints[u][v][1] = 2.0 * ((GLfloat) v - 1.5);

			if ((u == 1 || u == 2) && (v == 1 || v == 2)) ctlpoints[u][v][2] = 3.0;
			else ctlpoints[u][v][2] = -3.0;
		}
	}
}

void nurbsError(GLenum errorCode) {
	const GLubyte *estring = NULL;
	estring = gluErrorString(errorCode);
	fprintf (stderr, "Nurbs Error: %s\n", estring);
	exit(-1);
}

void init(void) {
	GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 100.0 };

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	init_surface();

	theNurb = gluNewNurbsRenderer();
	/* value should be set to be either GLU_NURBS_RENDERER or GLU_NURBS_TESSELLATOR.
	 * When set to GLU_NURBS_RENDERER, NURBS objects are tessellated into OpenGL
	 * primitives and sent to the pipeline for rendering. When set to GLU_NURBS_TESSELLATOR,
	 * NURBS objects are tessellated into OpenGL primitives but the vertices, normals, colors,
	 * and/or textures are retrieved back through a callback interface (see gluNurbsCallback).
	 * This allows the user to cache the tessellated results for further processing.
	 * The initial value is GLU_NURBS_RENDERER.
	 **/
	gluNurbsProperty(theNurb, GLU_NURBS_MODE, GLU_NURBS_RENDERER);
	/* When set to GLU_PATH_LENGTH, the surface is rendered so that the maximum length, in pixels,
	 * of the edges of the tessellation polygons is no greater than what is specified by GLU_SAMPLING_TOLERANCE.
	 * The initial value is GLU_PATH_LENGTH.
	 **/
	gluNurbsProperty(theNurb, GLU_SAMPLING_METHOD, GLU_PATH_LENGTH);
	/* Specifies the maximum length, in pixels or in object space length unit, to use when the
	 * sampling method is set to GLU_PATH_LENGTH or GLU_OBJECT_PATH_LENGTH. The NURBS code is
	 * conservative when rendering a curve or surface, so the actual length can be somewhat shorter.
	 * The initial value is 50.0 pixels.
	 **/
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
	/* value can be set to GLU_OUTLINE_POLYGON, GLU_FILL, or GLU_OUTLINE_PATCH.
	 * When GLU_NURBS_MODE is set to be GLU_NURBS_RENDERER, value defines how a
	 * & NURBS surface should be rendered. When value is set to GLU_FILL, the surface
	 * is rendered as a set of polygons. When value is set to GLU_OUTLINE_POLYGON,
	 * the NURBS library draws only the outlines of the polygons created by tessellation.
	 * When value is set to GLU_OUTLINE_PATCH just the outlines of patches and trim
	 * curves defined by the user are drawn.
	 * The initial value is GLU_FILL.
	 **/
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
	gluNurbsCallback(theNurb, GLU_ERROR, (GLvoid (*)()) nurbsError);
}

void display(void) {
	int i, j;
	GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
		glRotatef(330.0, 1.,0.,0.);
		glScalef (0.3, 0.3, 0.3);

		if (angx != 0) glRotated(angx, 1, 0, 0);
		if (angy != 0) glRotated(angy, 0, 1, 0);

		gluBeginSurface(theNurb);
			gluNurbsSurface(theNurb, 8, knots, 8, knots, 4 * 3, 3, &ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
		gluEndSurface(theNurb);

		if (showPoints) {
			glPointSize(5.0);
			glDisable(GL_LIGHTING); // why this?
			glColor3f(1.0, 1.0, 0.0);
			glBegin(GL_POINTS);
				for (i = 0; i < 4; i++)
					for (j = 0; j < 4; j++)
						glVertex3f(ctlpoints[i][j][0], ctlpoints[i][j][1], ctlpoints[i][j][2]);
			glEnd();
			glEnable(GL_LIGHTING);
		}
	glPopMatrix();
	glFlush();
}

void mouse(int button, int state, int x, int y) {
	if (button ==  GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN)
			angx += 15;

	if (button ==  GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN)
		angy += 15;

	glutPostRedisplay();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLdouble)w/(GLdouble)h, 3.0, 8.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'c':
		case 'C':
			showPoints = !showPoints;
			glutPostRedisplay();
			break;
		case 27: // ESC
			exit(0);
			break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500);
	glutInitWindowPosition (100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutDisplayFunc(display);
	glutKeyboardFunc (keyboard);
	glutMainLoop();
	return 0;
}
